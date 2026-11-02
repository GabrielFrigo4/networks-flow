#include "../../Implementações/CostNetwork/CycleCanceling.hpp"
#include "../../Implementações/CostNetwork/NetworkSimplex.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortest.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp"

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>

#include <psapi.h>
#else
#include <sys/resource.h>
#endif

struct RawCostArc
{
	Size u, v;
	Long cap, cost;
};

struct CostGraphData
{
	Size num_nodes = 0;
	Size source = 0;
	Size sink = 0;
	std::vector<RawCostArc> arcs;
};

struct CostAlgoResult
{
	std::string algo_name;
	Long min_cost = 0;
	double elapsed_ms = 0.0;
	long mem_kb = 0;
	std::string status = "OK";
};

static long get_max_rss_kb()
{
#ifdef _WIN32
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		return static_cast<long>(pmc.PeakWorkingSetSize / 1024);
	return 0;
#else
	struct rusage usage{};
	if (getrusage(RUSAGE_SELF, &usage) == 0)
		return usage.ru_maxrss;
	return 0;
#endif
}

static CostGraphData load_dimacs_min_cost(const std::string &filepath)
{
	std::ifstream in(filepath);
	if (!in.is_open())
	{
		std::cerr << "Erro ao abrir arquivo: " << filepath << "\n";
		std::exit(1);
	}

	CostGraphData data;
	Size orig_nodes = 0;
	std::vector<Long> supply;
	std::string line;
	while (std::getline(in, line))
	{
		if (line.empty() || line[0] == 'c')
			continue;

		std::istringstream iss(line);
		char type = 0;
		iss >> type;

		if (type == 'p')
		{
			std::string problem;
			Size num_arcs = 0;
			iss >> problem >> orig_nodes >> num_arcs;
			supply.assign(orig_nodes, 0);
			data.arcs.reserve(num_arcs + orig_nodes);
		}
		else if (type == 'n')
		{
			Size id = 0;
			Long sup = 0;
			iss >> id >> sup;
			if (id >= 1 && id <= orig_nodes)
				supply[id - 1] = sup;
		}
		else if (type == 'a')
		{
			Size u = 0, v = 0;
			Long low = 0, cap = 0, cost = 0;
			iss >> u >> v >> low >> cap >> cost;
			(void)low;
			data.arcs.push_back({u - 1, v - 1, cap, cost});
		}
	}

	data.source = orig_nodes;
	data.sink = orig_nodes + 1;
	data.num_nodes = orig_nodes + 2;

	for (Size u = 0; u < orig_nodes; ++u)
	{
		if (supply[u] > 0)
			data.arcs.push_back({data.source, u, supply[u], 0});
		else if (supply[u] < 0)
			data.arcs.push_back({u, data.sink, -supply[u], 0});
	}

	return data;
}

template <typename Solver>
static CostAlgoResult run_cost_solver_instance(const CostGraphData &data)
{
	const long rss_before = get_max_rss_kb();
	const auto t0 = std::chrono::steady_clock::now();

	auto net = Solver::create(data.num_nodes);
	for (const auto &arc : data.arcs)
		net->add_edge(arc.u, arc.v, arc.cap, arc.cost);

	const Long min_cost = net->compute_min_cost_max_flow(data.source, data.sink);

	const auto t1 = std::chrono::steady_clock::now();
	const long rss_after = get_max_rss_kb();
	const double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0)
	                              .count();
	const long mem_kb = std::max(rss_before, rss_after);

	return {"", min_cost, elapsed_ms, mem_kb, "OK"};
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Uso: " << argv[0]
		          << " <arquivo.min> [nome_instancia] [--algo nome]\n";
		return 1;
	}

	const std::string filepath = argv[1];
	std::string instance_name = filepath;
	std::string only_algo;

	for (int i = 2; i < argc; ++i)
	{
		const std::string arg = argv[i];
		if (arg == "--algo" && i + 1 < argc)
			only_algo = argv[++i];
		else if (!arg.empty() && arg[0] != '-')
			instance_name = arg;
	}

	const CostGraphData data = load_dimacs_min_cost(filepath);

	std::vector<std::pair<std::string, std::function<CostAlgoResult()>>> tasks;

	tasks.push_back({
	    "NetworkSimplex",
	    [&data]() { return run_cost_solver_instance<NetworkSimplex>(data); },
	});
	tasks.push_back({
	    "SuccessiveShortestDijkstra",
	    [&data]() {
		    return run_cost_solver_instance<SuccessiveShortestDijkstra>(data);
	    },
	});
	tasks.push_back({
	    "SuccessiveShortest",
	    [&data]() { return run_cost_solver_instance<SuccessiveShortest>(data); },
	});
	tasks.push_back({
	    "CycleCanceling",
	    [&data]() { return run_cost_solver_instance<CycleCanceling>(data); },
	});

	if (!only_algo.empty())
		std::erase_if(tasks, [&only_algo](const auto &task) {
			return task.first != only_algo;
		});

	if (tasks.empty())
	{
		std::cerr << "Algoritmo desconhecido: " << only_algo << "\n";
		return 1;
	}

	std::vector<CostAlgoResult> results(tasks.size());
	{
		std::vector<std::jthread> threads;
		threads.reserve(tasks.size());

		for (size_t idx = 0; idx < tasks.size(); ++idx)
		{
			threads.emplace_back([&tasks, &results, idx]() {
				auto res = tasks[idx].second();
				res.algo_name = tasks[idx].first;
				results[idx] = res;
			});
		}
	}

	Long reference_cost = 0;
	bool has_reference = false;

	for (const auto &res : results)
	{
		if (!has_reference)
		{
			reference_cost = res.min_cost;
			has_reference = true;
		}
		else if (res.min_cost != reference_cost)
		{
			std::cerr << "[ERRO DE CONSISTENCIA] " << res.algo_name
			          << " obteve custo " << res.min_cost << " != referencia "
			          << reference_cost << " em " << instance_name << "\n";
			std::exit(2);
		}
	}

	Size total_edges = data.arcs.size();
	for (const auto &res : results)
	{
		std::cout << instance_name << "," << res.algo_name << "," << data.num_nodes
		          << "," << total_edges << "," << res.min_cost << "," << std::fixed
		          << std::setprecision(3) << res.elapsed_ms << "," << res.mem_kb
		          << "," << res.status << "\n";
	}

	return 0;
}

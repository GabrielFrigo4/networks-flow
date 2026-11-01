#include "../../Implementações/FlowNetwork/Dinic.hpp"
#include "../../Implementações/FlowNetwork/EdmondsKarp.hpp"
#include "../../Implementações/FlowNetwork/FordFulkerson.hpp"
#include "../../Implementações/FlowNetwork/MPM.hpp"
#include "../../Implementações/FlowNetwork/PushRelabel.hpp"
#include "../../Implementações/FlowNetwork/PushRelabelImproved.hpp"

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

struct RawArc
{
	Size u, v;
	Long cap;
};

struct GraphData
{
	Size num_nodes = 0;
	Size source = 0;
	Size sink = 0;
	std::vector<RawArc> arcs;
};

struct AlgoResult
{
	std::string algo_name;
	Long flow = 0;
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

static GraphData load_dimacs_max_flow(const std::string &filepath)
{
	std::ifstream in(filepath);
	if (!in.is_open())
	{
		std::cerr << "Erro ao abrir arquivo: " << filepath << "\n";
		std::exit(1);
	}

	GraphData data;
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
			iss >> problem >> data.num_nodes >> num_arcs;
			data.arcs.reserve(num_arcs);
		}
		else if (type == 'n')
		{
			Size id = 0;
			char role = 0;
			iss >> id >> role;
			if (role == 's')
				data.source = id - 1;
			else if (role == 't')
				data.sink = id - 1;
		}
		else if (type == 'a')
		{
			Size u = 0, v = 0;
			Long cap = 0;
			iss >> u >> v >> cap;
			data.arcs.push_back({u - 1, v - 1, cap});
		}
	}

	return data;
}

template <typename Solver>
static AlgoResult run_solver_instance(const GraphData &data)
{
	const long rss_before = get_max_rss_kb();
	const auto t0 = std::chrono::steady_clock::now();

	auto net = Solver::create(data.num_nodes);
	for (const auto &arc : data.arcs)
		net->add_edge(arc.u, arc.v, arc.cap);

	const Long flow = net->compute_max_flow(data.source, data.sink);

	const auto t1 = std::chrono::steady_clock::now();
	const long rss_after = get_max_rss_kb();
	const double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0)
	                              .count();
	const long mem_kb = std::max(rss_before, rss_after);

	return {"", flow, elapsed_ms, mem_kb, "OK"};
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Uso: " << argv[0]
		          << " <arquivo.max> [nome_instancia] [--suite ic|book] [--algo "
		             "nome]\n";
		return 1;
	}

	const std::string filepath = argv[1];
	std::string instance_name = filepath;
	std::string suite = "book";
	std::string only_algo;

	for (int i = 2; i < argc; ++i)
	{
		const std::string arg = argv[i];
		if (arg == "--suite" && i + 1 < argc)
			suite = argv[++i];
		else if (arg == "--algo" && i + 1 < argc)
			only_algo = argv[++i];
		else if (!arg.empty() && arg[0] != '-')
			instance_name = arg;
	}

	const GraphData data = load_dimacs_max_flow(filepath);

	auto should_run = [&](const std::string &name) {
		if (!only_algo.empty())
			return only_algo == name;
		if (name == "MPM")
			return suite == "book";
		return true;
	};

	std::vector<std::pair<std::string, std::function<AlgoResult()>>> tasks;

	if (should_run("Dinic"))
		tasks.push_back({"Dinic", [&data]() {
			                 return run_solver_instance<Dinic>(data);
		                 }});
	if (should_run("PushRelabelImproved"))
		tasks.push_back({
		    "PushRelabelImproved",
		    [&data]() { return run_solver_instance<PushRelabelImproved>(data); },
		});
	if (should_run("PushRelabel"))
		tasks.push_back({
		    "PushRelabel",
		    [&data]() { return run_solver_instance<PushRelabel>(data); },
		});
	if (should_run("EdmondsKarp"))
		tasks.push_back({
		    "EdmondsKarp",
		    [&data]() { return run_solver_instance<EdmondsKarp>(data); },
		});
	if (should_run("FordFulkerson"))
		tasks.push_back({
		    "FordFulkerson",
		    [&data]() { return run_solver_instance<FordFulkerson>(data); },
		});
	if (should_run("MPM"))
		tasks.push_back({"MPM", [&data]() {
			                 return run_solver_instance<MPM>(data);
		                 }});

	std::vector<AlgoResult> results(tasks.size());
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

	Long reference_flow = 0;
	bool has_reference = false;

	for (const auto &res : results)
	{
		if (!has_reference)
		{
			reference_flow = res.flow;
			has_reference = true;
		}
		else if (res.flow != reference_flow)
		{
			std::cerr << "[ERRO DE CONSISTENCIA] " << res.algo_name
			          << " obteve fluxo " << res.flow << " != referencia "
			          << reference_flow << " em " << instance_name << "\n";
			std::exit(2);
		}
	}

	for (const auto &res : results)
	{
		std::cout << instance_name << "," << res.algo_name << "," << data.num_nodes
		          << "," << data.arcs.size() << "," << res.flow << "," << std::fixed
		          << std::setprecision(3) << res.elapsed_ms << "," << res.mem_kb
		          << "," << res.status << "\n";
	}

	return 0;
}

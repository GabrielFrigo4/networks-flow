#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <vector>

#include "../../Implementações/FlowNetwork/Dinic.hpp"
#include "../../Implementações/FlowNetwork/EdmondsKarp.hpp"
#include "../../Implementações/FlowNetwork/FordFulkerson.hpp"
#include "../../Implementações/FlowNetwork/HopcroftKarp.hpp"
#include "../../Implementações/FlowNetwork/Kuhn.hpp"
#include "../../Implementações/FlowNetwork/MPM.hpp"
#include "../../Implementações/FlowNetwork/PushRelabel.hpp"
#include "../../Implementações/FlowNetwork/PushRelabelImproved.hpp"

namespace CostBench
{
#include "../../Implementações/CostNetwork/CycleCanceling.hpp"
#include "../../Implementações/CostNetwork/KuhnMunkres.hpp"
#include "../../Implementações/CostNetwork/NetworkSimplex.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortest.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp"
} // namespace CostBench

struct RawFlowArc
{
	Size u, v;
	Long cap;
};

struct FlowGraphData
{
	Size num_nodes = 0;
	Size source = 0;
	Size sink = 0;
	std::vector<RawFlowArc> arcs;
};

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

static long get_max_rss_kb()
{
	struct rusage usage{};
	if (getrusage(RUSAGE_SELF, &usage) == 0)
		return usage.ru_maxrss;
	return 0;
}

static FlowGraphData load_dimacs_matching(const std::string &filepath)
{
	std::ifstream in(filepath);
	if (!in.is_open())
	{
		std::cerr << "Erro ao abrir arquivo: " << filepath << "\n";
		std::exit(1);
	}

	FlowGraphData data;
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

static CostGraphData load_dimacs_assignment(const std::string &filepath)
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
static void run_matching_solver(
    const std::string &instance_name, const std::string &algo_name,
    const FlowGraphData &data, Long &reference_flow, bool &has_reference
)
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

	if (!has_reference)
	{
		reference_flow = flow;
		has_reference = true;
	}
	else if (flow != reference_flow)
	{
		std::cerr << "[ERRO DE CONSISTENCIA BIPARTIDO] " << algo_name
		          << " obteve matching " << flow << " != referencia "
		          << reference_flow << " em " << instance_name << "\n";
		std::exit(2);
	}

	const long mem_kb = std::max(rss_before, rss_after);

	std::cout << instance_name << "," << algo_name << "," << data.num_nodes << ","
	          << data.arcs.size() << "," << flow << "," << std::fixed
	          << std::setprecision(3) << elapsed_ms << "," << mem_kb << ",OK\n";
}

template <typename Solver>
static void run_assignment_solver(
    const std::string &instance_name, const std::string &algo_name,
    const CostGraphData &data, Long &reference_cost, bool &has_reference
)
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

	if (!has_reference)
	{
		reference_cost = min_cost;
		has_reference = true;
	}
	else if (min_cost != reference_cost)
	{
		std::cerr << "[ERRO DE CONSISTENCIA ASSIGNMENT] " << algo_name
		          << " obteve custo " << min_cost << " != referencia "
		          << reference_cost << " em " << instance_name << "\n";
		std::exit(2);
	}

	const long mem_kb = std::max(rss_before, rss_after);

	std::cout << instance_name << "," << algo_name << "," << data.num_nodes << ","
	          << data.arcs.size() << "," << min_cost << "," << std::fixed
	          << std::setprecision(3) << elapsed_ms << "," << mem_kb << ",OK\n";
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Uso: " << argv[0]
		          << " <arquivo> [nome_instancia] [--mode matching|assignment]\n";
		return 1;
	}

	const std::string filepath = argv[1];
	std::string instance_name = filepath;
	std::string mode = "matching";

	for (int i = 2; i < argc; ++i)
	{
		const std::string arg = argv[i];
		if (arg == "--mode" && i + 1 < argc)
			mode = argv[++i];
		else if (!arg.empty() && arg[0] != '-')
			instance_name = arg;
	}

	if (mode == "assignment")
	{
		const CostGraphData data = load_dimacs_assignment(filepath);
		Long reference_cost = 0;
		bool has_reference = false;

		run_assignment_solver<CostBench::KuhnMunkres>(
		    instance_name, "KuhnMunkres", data, reference_cost, has_reference
		);
		run_assignment_solver<CostBench::NetworkSimplex>(
		    instance_name, "NetworkSimplex", data, reference_cost, has_reference
		);
		run_assignment_solver<CostBench::SuccessiveShortestDijkstra>(
		    instance_name,
		    "SuccessiveShortestDijkstra",
		    data,
		    reference_cost,
		    has_reference
		);
		run_assignment_solver<CostBench::SuccessiveShortest>(
		    instance_name, "SuccessiveShortest", data, reference_cost, has_reference
		);
		run_assignment_solver<CostBench::CycleCanceling>(
		    instance_name, "CycleCanceling", data, reference_cost, has_reference
		);
	}
	else
	{
		const FlowGraphData data = load_dimacs_matching(filepath);
		Long reference_flow = 0;
		bool has_reference = false;

		run_matching_solver<HopcroftKarp>(
		    instance_name, "HopcroftKarp", data, reference_flow, has_reference
		);
		run_matching_solver<Kuhn>(
		    instance_name, "Kuhn", data, reference_flow, has_reference
		);
		run_matching_solver<Dinic>(
		    instance_name, "Dinic", data, reference_flow, has_reference
		);
		run_matching_solver<PushRelabelImproved>(
		    instance_name, "PushRelabelImproved", data, reference_flow, has_reference
		);
		run_matching_solver<PushRelabel>(
		    instance_name, "PushRelabel", data, reference_flow, has_reference
		);
		run_matching_solver<MPM>(
		    instance_name, "MPM", data, reference_flow, has_reference
		);
		run_matching_solver<EdmondsKarp>(
		    instance_name, "EdmondsKarp", data, reference_flow, has_reference
		);
		run_matching_solver<FordFulkerson>(
		    instance_name, "FordFulkerson", data, reference_flow, has_reference
		);
	}

	return 0;
}

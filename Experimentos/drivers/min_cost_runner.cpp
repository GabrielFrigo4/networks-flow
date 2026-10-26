#include "../../Implementações/CostNetwork/CycleCanceling.hpp"
#include "../../Implementações/CostNetwork/NetworkSimplex.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortest.hpp"
#include "../../Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <vector>

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
static void run_solver(
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
		std::cerr << "[ERRO DE CONSISTENCIA MCF] " << algo_name << " obteve custo "
		          << min_cost << " != referencia " << reference_cost << " em "
		          << instance_name << "\n";
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
		std::cerr << "Uso: " << argv[0] << " <arquivo.min> [nome_instancia]\n";
		return 1;
	}

	const std::string filepath = argv[1];
	const std::string instance_name = (argc >= 3) ? argv[2] : filepath;

	const CostGraphData data = load_dimacs_min_cost(filepath);

	Long reference_cost = 0;
	bool has_reference = false;

	run_solver<NetworkSimplex>(
	    instance_name, "NetworkSimplex", data, reference_cost, has_reference
	);
	run_solver<SuccessiveShortestDijkstra>(
	    instance_name,
	    "SuccessiveShortestDijkstra",
	    data,
	    reference_cost,
	    has_reference
	);
	run_solver<SuccessiveShortest>(
	    instance_name, "SuccessiveShortest", data, reference_cost, has_reference
	);
	run_solver<CycleCanceling>(
	    instance_name, "CycleCanceling", data, reference_cost, has_reference
	);

	return 0;
}

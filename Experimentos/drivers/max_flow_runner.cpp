#include "../../Implementações/FlowNetwork/Dinic.hpp"
#include "../../Implementações/FlowNetwork/EdmondsKarp.hpp"
#include "../../Implementações/FlowNetwork/FordFulkerson.hpp"
#include "../../Implementações/FlowNetwork/MPM.hpp"
#include "../../Implementações/FlowNetwork/PushRelabel.hpp"
#include "../../Implementações/FlowNetwork/PushRelabelImproved.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <vector>

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

static long get_max_rss_kb()
{
	struct rusage usage{};
	if (getrusage(RUSAGE_SELF, &usage) == 0)
		return usage.ru_maxrss;
	return 0;
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
static void run_solver(
    const std::string &instance_name, const std::string &algo_name,
    const GraphData &data, Long &reference_flow, bool &has_reference
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
		std::cerr << "[ERRO DE CONSISTENCIA] " << algo_name << " obteve fluxo "
		          << flow << " != referencia " << reference_flow << " em "
		          << instance_name << "\n";
		std::exit(2);
	}

	const long mem_kb = std::max(rss_before, rss_after);

	std::cout << instance_name << "," << algo_name << "," << data.num_nodes << ","
	          << data.arcs.size() << "," << flow << "," << std::fixed
	          << std::setprecision(3) << elapsed_ms << "," << mem_kb << ",OK\n";
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

	Long reference_flow = 0;
	bool has_reference = false;

	auto should_run = [&](const std::string &name) {
		if (!only_algo.empty())
			return only_algo == name;
		if (name == "MPM")
			return suite == "book";
		return true;
	};

	if (should_run("Dinic"))
		run_solver<Dinic>(
		    instance_name, "Dinic", data, reference_flow, has_reference
		);
	if (should_run("PushRelabelImproved"))
		run_solver<PushRelabelImproved>(
		    instance_name, "PushRelabelImproved", data, reference_flow, has_reference
		);
	if (should_run("PushRelabel"))
		run_solver<PushRelabel>(
		    instance_name, "PushRelabel", data, reference_flow, has_reference
		);
	if (should_run("EdmondsKarp"))
		run_solver<EdmondsKarp>(
		    instance_name, "EdmondsKarp", data, reference_flow, has_reference
		);
	if (should_run("FordFulkerson"))
		run_solver<FordFulkerson>(
		    instance_name, "FordFulkerson", data, reference_flow, has_reference
		);
	if (should_run("MPM"))
		run_solver<MPM>(instance_name, "MPM", data, reference_flow, has_reference);

	return 0;
}

// ----------------------------------------------------------------
// Driver: Min-Cost Flow Benchmark Runner
// ----------------------------------------------------------------
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../Experimentos/parsers/dimacs_min.hpp"
#include "CostNetwork/CycleCanceling.hpp"
#include "CostNetwork/NetworkSimplex.hpp"
#include "CostNetwork/SuccessiveShortest.hpp"
#include "CostNetwork/SuccessiveShortestDijkstra.hpp"

struct EngineEntry
{
	std::string name;
	std::unique_ptr<CostNetwork> (*create)(Size);
};

static const std::vector<EngineEntry> &get_engines()
{
	static const auto engines = [] {
		std::vector<EngineEntry> v;
		v.push_back({"CycleCanceling", CycleCanceling::create});
		v.push_back({"SuccessiveShortest", SuccessiveShortest::create});
		v.push_back(
		    {"SuccessiveShortestDijkstra", SuccessiveShortestDijkstra::create}
		);
		v.push_back({"NetworkSimplex", NetworkSimplex::create});
		return v;
	}();
	return engines;
}

static std::unique_ptr<CostNetwork> build_network(
    const EngineEntry &engine, const DimacsMinCost &inst
)
{
	auto net = engine.create(inst.num_nodes);
	for (const auto &arc : inst.arcs)
		net->add_edge(arc.from, arc.to, arc.capacity, arc.cost);
	return net;
}

struct RunResult
{
	Long cost_value;
	Long flow_value;
	double elapsed_ms;
	bool timeout;
};

static RunResult run_once(
    const EngineEntry &engine, const DimacsMinCost &inst, double timeout_s
)
{
	auto net = build_network(engine, inst);

	std::packaged_task<std::pair<Long, Long>()> task(
	    [n = std::move(net), &inst]() mutable -> std::pair<Long, Long> {
		    Long cost = n->compute_min_cost_max_flow(inst.source, inst.sink);
		    Long flow = n->get_total_flow(inst.source);
		    return {cost, flow};
	    }
	);
	auto future = task.get_future();

	auto t0 = std::chrono::high_resolution_clock::now();
	std::thread worker(std::move(task));

	if (future.wait_for(std::chrono::duration<double>(timeout_s)) ==
	    std::future_status::timeout)
	{
		worker.detach();
		auto t1 = std::chrono::high_resolution_clock::now();
		double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
		return {-1, -1, ms, true};
	}

	worker.join();
	auto t1 = std::chrono::high_resolution_clock::now();
	double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
	auto [cost, flow] = future.get();
	return {cost, flow, ms, false};
}

struct BenchResult
{
	std::string algorithm;
	Size n, m;
	Long flow_value;
	Long cost_value;
	double mean_ms;
	double stddev_ms;
	std::string status;
};

static BenchResult benchmark_engine(
    const EngineEntry &engine, const DimacsMinCost &inst, int repeats,
    double timeout_s
)
{
	BenchResult res{};
	res.algorithm = engine.name;
	res.n = inst.num_nodes;
	res.m = inst.arcs.size();
	res.flow_value = 0;
	res.cost_value = 0;
	res.mean_ms = 0;
	res.stddev_ms = 0;

	std::vector<double> times;
	times.reserve(static_cast<Size>(repeats));

	for (int r = 0; r < repeats; ++r)
	{
		auto result = run_once(engine, inst, timeout_s);

		if (result.timeout)
		{
			res.flow_value = result.flow_value;
			res.cost_value = result.cost_value;
			res.mean_ms = result.elapsed_ms;
			res.stddev_ms = 0;
			res.status = "TLE";
			return res;
		}

		res.flow_value = result.flow_value;
		res.cost_value = result.cost_value;
		times.push_back(result.elapsed_ms);
	}

	double sum = std::accumulate(times.begin(), times.end(), 0.0);
	res.mean_ms = sum / static_cast<double>(times.size());

	double sq_sum = 0.0;
	for (double t : times)
	{
		double diff = t - res.mean_ms;
		sq_sum += diff * diff;
	}
	res.stddev_ms = std::sqrt(sq_sum / static_cast<double>(times.size()));
	res.status = "OK";

	return res;
}

static std::string extract_filename(const std::string &path)
{
	auto pos = path.find_last_of("/\\");
	if (pos == std::string::npos)
		return path;
	return path.substr(pos + 1);
}

int main(int argc, char *argv[])
{
	int repeats = 5;
	double timeout_s = 60.0;
	bool csv_header = true;
	std::string filepath;

	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--repeats" && i + 1 < argc)
			repeats = std::atoi(argv[++i]);
		else if (arg == "--timeout" && i + 1 < argc)
			timeout_s = std::atof(argv[++i]);
		else if (arg == "--no-header")
			csv_header = false;
		else
			filepath = arg;
	}

	if (filepath.empty())
	{
		std::cerr << "Uso: min_cost_runner <arquivo.min> "
		          << "[--repeats N] [--timeout S] [--no-header]\n";
		return 1;
	}

	std::ifstream file(filepath);
	if (!file.is_open())
	{
		std::cerr << "Erro: não foi possível abrir '" << filepath << "'\n";
		return 1;
	}

	DimacsMinCost inst = parse_dimacs_min(file);
	file.close();

	std::string instance_name = extract_filename(filepath);

	if (csv_header)
	{
		std::cout << "instance,algorithm,n,m,flow_value,"
		          << "cost_value,mean_ms,stddev_ms,status\n";
	}

	Long reference_cost = -1;
	Long reference_flow = -1;
	bool cross_validation_ok = true;

	for (const auto &engine : get_engines())
	{
		std::cerr << "  [" << instance_name << "] " << engine.name << "..."
		          << std::flush;

		auto res = benchmark_engine(engine, inst, repeats, timeout_s);

		std::cout << instance_name << "," << res.algorithm << "," << res.n << ","
		          << res.m << "," << res.flow_value << "," << res.cost_value << ","
		          << res.mean_ms << "," << res.stddev_ms << "," << res.status
		          << "\n";

		std::cerr << " " << res.mean_ms << " ms"
		          << " [" << res.status << "]\n";

		if (res.status == "OK")
		{
			if (reference_cost < 0)
			{
				reference_cost = res.cost_value;
				reference_flow = res.flow_value;
			}
			else
			{
				if (res.cost_value != reference_cost ||
				    res.flow_value != reference_flow)
					cross_validation_ok = false;
			}
		}
	}

	if (!cross_validation_ok)
	{
		std::cerr << "ERRO: Verificação cruzada falhou! "
		          << "Valores de z* ou f* divergentes.\n";
		return 2;
	}

	std::cerr << "Verificação cruzada: OK (z* = " << reference_cost
	          << ", f* = " << reference_flow << ")\n";
	return 0;
}

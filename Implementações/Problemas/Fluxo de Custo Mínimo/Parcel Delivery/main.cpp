#include "../../../CostNetwork/CycleCanceling.hpp"
#include "../../../CostNetwork/NetworkSimplex.hpp"
#include "../../../CostNetwork/SuccessiveShortest.hpp"
#include "../../../CostNetwork/SuccessiveShortestDijkstra.hpp"
#include <iostream>

using CostSolver = NetworkSimplex;

void task()
{
	Size num_cities, num_routes;
	Long num_parcels;
	if (!(std::cin >> num_cities >> num_routes >> num_parcels))
		return;

	const Size total_nodes = num_cities + 1;
	const Size source = 0;
	const Size real_source = 1;
	const Size sink = num_cities;

	const auto cn = CostSolver::create(total_nodes);

	cn->add_edge(source, real_source, num_parcels, 0);

	for (Size i = 0; i < num_routes; i++)
	{
		Size from_city, to_city;
		Long capacity, cost;
		std::cin >> from_city >> to_city >> capacity >> cost;
		cn->add_edge(from_city, to_city, capacity, cost);
	}

	const Long min_cost = cn->compute_min_cost_max_flow(source, sink);
	const Long total_flow = cn->get_total_flow(source);

	if (total_flow < num_parcels)
	{
		std::cout << -1 << std::endl;
	}
	else
	{
		std::cout << min_cost << std::endl;
	}
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

#include "../../../CostNetwork/CycleCanceling.hpp"
#include "../../../CostNetwork/NetworkSimplex.hpp"
#include "../../../CostNetwork/SuccessiveShortest.hpp"
#include "../../../CostNetwork/SuccessiveShortestDijkstra.hpp"
#include <iostream>

using CostSolver = NetworkSimplex;

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const auto cn = CostSolver::create(num_nodes);
	for (Size k = 0; k < num_edges; k++)
	{
		Size from_node, to_node;
		Long capacity, cost;
		std::cin >> from_node >> to_node >> capacity >> cost;
		cn->add_edge(from_node - 1, to_node - 1, capacity, cost);
	}

	std::cout << cn->compute_min_cost_max_flow(0, num_nodes - 1) << std::endl;
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

#include "../../../CostNetwork/CycleCanceling.hpp"
#include "../../../CostNetwork/NetworkSimplex.hpp"
#include "../../../CostNetwork/SuccessiveShortest.hpp"
#include "../../../CostNetwork/SuccessiveShortestDijkstra.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

using CostSolver = NetworkSimplex;

void print_disjoint_paths(
    const std::unique_ptr<CostNetwork> &cn, const Size num_nodes, const Long k
)
{
	const auto &edges = cn->get_edges();
	const auto &adjacency = cn->get_adjacency();
	std::vector<bool> used_edge(edges.size(), false);

	for (Long i = 0; i < k; i++)
	{
		std::vector<Size> path;
		Size curr = 1;
		path.push_back(curr);

		while (curr != num_nodes)
		{
			for (const Size edge_id : adjacency[curr])
			{
				if (edge_id % 2 != 0 || edges[edge_id].flow <= 0 ||
				    used_edge[edge_id])
				{
					continue;
				}

				used_edge[edge_id] = true;
				curr = edges[edge_id].to;
				path.push_back(curr);
				break;
			}
		}

		std::cout << path.size() << std::endl;
		for (Size j = 0; j < path.size(); j++)
		{
			std::cout << path[j] << (j + 1 == path.size() ? "" : " ");
		}
		std::cout << std::endl;
	}
}

void task()
{
	Size num_nodes, num_edges;
	Long num_paths;
	if (!(std::cin >> num_nodes >> num_edges >> num_paths))
		return;

	const Size total_nodes = num_nodes + 1;
	const Size source = 0;
	const Size real_source = 1;
	const Size sink = num_nodes;

	const auto cn = CostSolver::create(total_nodes);

	cn->add_edge(source, real_source, num_paths, 0);

	for (Size i = 0; i < num_edges; i++)
	{
		Size from_node, to_node;
		std::cin >> from_node >> to_node;
		cn->add_edge(from_node, to_node, 1, 1);
	}

	const Long min_cost = cn->compute_min_cost_max_flow(source, sink);
	const Long total_flow = cn->get_total_flow(source);

	if (total_flow < num_paths)
	{
		std::cout << -1 << std::endl;
		return;
	}

	std::cout << min_cost << std::endl;
	print_disjoint_paths(cn, num_nodes, num_paths);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

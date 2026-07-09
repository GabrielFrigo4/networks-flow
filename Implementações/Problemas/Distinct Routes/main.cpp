// Problema: Distinct Routes (CSES 1711)
// https://cses.fi/problemset/task/1711
//
// Caminhos Disjuntos por Arcos com capacidades unitárias e decomposição de fluxo.

#include "../../Dinic.hpp"
#include "../../EdmondsKarp.hpp"
#include "../../FordFulkerson.hpp"
#include "../../PushRelabel.hpp"
#include "../../PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

void print_disjoint_paths(
    const std::unique_ptr<FlowNetwork> &fn, const Size num_nodes, const Long max_flow
)
{
	const auto &edges = fn->get_edges();
	const auto &adjacency = fn->get_adjacency();
	std::vector<bool> used_edge(edges.size(), false);

	for (Long i = 0; i < max_flow; i++)
	{
		std::vector<Size> path;
		Size curr = 0;
		path.push_back(curr);

		while (curr != num_nodes - 1)
		{
			for (const Size edge_id : adjacency[curr])
			{
				if (edge_id % 2 != 0 || edges[edge_id].flow == 0 ||
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
			std::cout << path[j] + 1 << (j + 1 == path.size() ? "" : " ");
		}
		std::cout << std::endl;
	}
}

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const auto fn = FlowSolver::create(num_nodes);
	for (Size k = 0; k < num_edges; k++)
	{
		Size from_node, to_node;
		std::cin >> from_node >> to_node;
		fn->add_edge(from_node - 1, to_node - 1, 1);
	}

	const Long max_flow = fn->compute_max_flow(0, num_nodes - 1);
	std::cout << max_flow << std::endl;

	print_disjoint_paths(fn, num_nodes, max_flow);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

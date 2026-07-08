// Problema: Time Travel (Beecrowd 2082)
// https://judge.beecrowd.com/en/problems/view/2082
//
// Corte Mínimo Global via iteração sobre todos os pares (s, t).

#include "../../Dinic.hpp"
#include "../../EdmondsKarp.hpp"
#include "../../FordFulkerson.hpp"
#include "../../PushRelabel.hpp"
#include "../../PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

Long compute_global_min_cut(
    const std::unique_ptr<FlowNetwork> &fn, const Size num_nodes
)
{
	Long global_min_cut = INF;
	for (Size current_sink = 1; current_sink < num_nodes; current_sink++)
	{
		const auto fn_clone = fn->clone();
		const Long current_flow = fn_clone->compute_max_flow(0, current_sink);
		global_min_cut = std::min(global_min_cut, current_flow);
	}
	return global_min_cut;
}

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	std::vector<std::vector<Long>> capacity_matrix(
	    num_nodes, std::vector<Long>(num_nodes, 0)
	);

	for (Size k = 0; k < num_edges; k++)
	{
		Size from_node, to_node;
		Long capacity;
		std::cin >> from_node >> to_node >> capacity;

		from_node--;
		to_node--;

		capacity_matrix[from_node][to_node] = capacity;
		capacity_matrix[to_node][from_node] = capacity;
	}

	const auto fn = FlowSolver::create(num_nodes);

	for (Size i = 0; i < num_nodes; i++)
	{
		for (Size j = i + 1; j < num_nodes; j++)
		{
			if (capacity_matrix[i][j] <= 0)
			{
				continue;
			}

			fn->add_edge(i, j, capacity_matrix[i][j], capacity_matrix[i][j]);
		}
	}

	const Long global_min_cut = compute_global_min_cut(fn, num_nodes);
	std::cout << global_min_cut << std::endl;
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	Long num_test_cases;
	if (!(std::cin >> num_test_cases))
		return -1;

	while (num_test_cases--)
	{
		task();
	}
	return 0;
}

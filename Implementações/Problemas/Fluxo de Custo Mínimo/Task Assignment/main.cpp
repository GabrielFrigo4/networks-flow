#include "../../../CostNetwork/CycleCanceling.hpp"
#include "../../../CostNetwork/NetworkSimplex.hpp"
#include "../../../CostNetwork/SuccessiveShortest.hpp"
#include "../../../CostNetwork/SuccessiveShortestDijkstra.hpp"
#include <iostream>

using CostSolver = NetworkSimplex;

void print_assignment(const std::unique_ptr<CostNetwork> &cn, const Size n)
{
	std::vector<Size> assignment(n + 1);

	for (const auto &edge : cn->get_edges())
	{
		const bool from_employee = (edge.from >= 1 && edge.from <= n);
		const bool to_task = (edge.to > n && edge.to <= 2 * n);

		if (!from_employee || !to_task || edge.flow != 1)
		{
			continue;
		}

		assignment[edge.from] = edge.to - n;
	}

	for (Size i = 1; i <= n; i++)
	{
		std::cout << assignment[i] << (i == n ? "" : " ");
	}
	std::cout << std::endl;
}

void task()
{
	Size n;
	if (!(std::cin >> n))
		return;

	const Size total_nodes = 2 * n + 2;
	const Size source = 0;
	const Size sink = total_nodes - 1;

	const auto cn = CostSolver::create(total_nodes);

	for (Size i = 1; i <= n; i++)
		cn->add_edge(source, i, 1, 0);

	for (Size j = 1; j <= n; j++)
		cn->add_edge(n + j, sink, 1, 0);

	for (Size i = 1; i <= n; i++)
	{
		for (Size j = 1; j <= n; j++)
		{
			Long cost;
			std::cin >> cost;
			cn->add_edge(i, n + j, 1, cost);
		}
	}

	std::cout << cn->compute_min_cost_max_flow(source, sink) << std::endl;
	print_assignment(cn, n);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

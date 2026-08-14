#include "../../FlowNetwork/Dinic.hpp"
#include "../../FlowNetwork/EdmondsKarp.hpp"
#include "../../FlowNetwork/FordFulkerson.hpp"
#include "../../FlowNetwork/PushRelabel.hpp"
#include "../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>
#include <string>

using FlowSolver = PushRelabelImproved;

void print_minimum_vertex_cover(
    const std::unique_ptr<FlowNetwork> &fn, const Size n
)
{
	const Size source = 0;
	std::vector<bool> reachable(2 * n + 2, false);
	std::queue<Size> q;

	q.push(source);
	reachable[source] = true;

	while (!q.empty())
	{
		const Size u = q.front();
		q.pop();

		for (const Size edge_id : fn->get_adjacency()[u])
		{
			const auto &edge = fn->get_edges()[edge_id];

			if (edge.capacity - edge.flow <= 0 || reachable[edge.to])
			{
				continue;
			}

			reachable[edge.to] = true;
			q.push(edge.to);
		}
	}

	for (Size i = 1; i <= n; i++)
	{
		if (!reachable[i])
			std::cout << "1 " << i << std::endl;
	}
	for (Size j = 1; j <= n; j++)
	{
		if (reachable[n + j])
			std::cout << "2 " << j << std::endl;
	}
}

void task()
{
	Size n;
	if (!(std::cin >> n))
		return;

	const Size total_nodes = 2 * n + 2;
	const Size source = 0;
	const Size sink = total_nodes - 1;

	const auto fn = FlowSolver::create(total_nodes);

	for (Size i = 1; i <= n; i++)
		fn->add_edge(source, i, 1);

	for (Size j = 1; j <= n; j++)
		fn->add_edge(n + j, sink, 1);

	for (Size i = 1; i <= n; i++)
	{
		std::string row;
		std::cin >> row;
		for (Size j = 1; j <= n; j++)
		{
			if (row[j - 1] == 'o')
				fn->add_edge(i, n + j, 1);
		}
	}

	std::cout << fn->compute_max_flow(source, sink) << std::endl;
	print_minimum_vertex_cover(fn, n);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

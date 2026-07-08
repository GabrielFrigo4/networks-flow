// Problema: Police Chase (CSES 1695)
// https://cses.fi/problemset/task/1695
//
// Corte Mínimo via Fluxo Máximo-Corte Mínimo (Max-Flow Min-Cut).

#include "../../Dinic.hpp"
#include "../../EdmondsKarp.hpp"
#include "../../FordFulkerson.hpp"
#include "../../PushRelabel.hpp"
#include "../../PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

std::vector<bool> get_reachable_nodes(
    const std::unique_ptr<FlowNetwork> &fn, const Size num_nodes
)
{
	std::vector<bool> reachable(num_nodes, false);
	std::queue<Size> q;

	q.push(0);
	reachable[0] = true;

	while (!q.empty())
	{
		const Size u = q.front();
		q.pop();

		for (const Size edge_id : fn->get_adj()[u])
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

	return reachable;
}

void print_min_cut_edges(
    const std::unique_ptr<FlowNetwork> &fn, const std::vector<bool> &reachable
)
{
	for (const auto &edge : fn->get_edges())
	{
		if (!reachable[edge.from] || reachable[edge.to] || edge.capacity <= 0)
		{
			continue;
		}

		std::cout << edge.from + 1 << " " << edge.to + 1 << std::endl;
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
		Size u, v;
		std::cin >> u >> v;
		fn->add_edge(u - 1, v - 1, 1, 1);
	}

	const Long max_flow = fn->compute_max_flow(0, num_nodes - 1);
	std::cout << max_flow << std::endl;

	const std::vector<bool> reachable = get_reachable_nodes(fn, num_nodes);
	print_min_cut_edges(fn, reachable);
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

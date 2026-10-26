#include "../../../FlowNetwork/Dinic.hpp"
#include "../../../FlowNetwork/EdmondsKarp.hpp"
#include "../../../FlowNetwork/FordFulkerson.hpp"
#include "../../../FlowNetwork/HopcroftKarp.hpp"
#include "../../../FlowNetwork/Kuhn.hpp"
#include "../../../FlowNetwork/MPM.hpp"
#include "../../../FlowNetwork/PushRelabel.hpp"
#include "../../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = MPM;

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const Size source = 0;
	const Size sink = num_nodes + num_edges + 1;
	const auto fn = FlowSolver::create(sink + 1);

	for (Size i = 1; i <= num_nodes; ++i)
	{
		Long vertex_cost;
		std::cin >> vertex_cost;
		fn->add_edge(i, sink, vertex_cost);
	}

	Long total_edge_weight = 0;
	for (Size j = 1; j <= num_edges; ++j)
	{
		Size u, v;
		Long weight;
		std::cin >> u >> v >> weight;

		const Size edge_node = num_nodes + j;
		total_edge_weight += weight;
		fn->add_edge(source, edge_node, weight);
		fn->add_edge(edge_node, u, INF);
		fn->add_edge(edge_node, v, INF);
	}

	const Long min_cut = fn->compute_max_flow(source, sink);
	std::cout << (total_edge_weight - min_cut) << '\n';
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

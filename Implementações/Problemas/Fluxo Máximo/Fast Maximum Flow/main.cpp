#include "../../../FlowNetwork/Dinic.hpp"
#include "../../../FlowNetwork/EdmondsKarp.hpp"
#include "../../../FlowNetwork/FordFulkerson.hpp"
#include "../../../FlowNetwork/HopcroftKarp.hpp"
#include "../../../FlowNetwork/Kuhn.hpp"
#include "../../../FlowNetwork/MPM.hpp"
#include "../../../FlowNetwork/PushRelabel.hpp"
#include "../../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const auto fn = FlowSolver::create(num_nodes);
	for (Size k = 0; k < num_edges; ++k)
	{
		Size u, v;
		Long cap;
		std::cin >> u >> v >> cap;
		if (u == v)
			continue;
		fn->add_edge(u - 1, v - 1, cap);
		fn->add_edge(v - 1, u - 1, cap);
	}

	std::cout << fn->compute_max_flow(0, num_nodes - 1) << '\n';
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

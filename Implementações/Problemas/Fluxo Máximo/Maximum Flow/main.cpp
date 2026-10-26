#include "../../../FlowNetwork/Dinic.hpp"
#include "../../../FlowNetwork/EdmondsKarp.hpp"
#include "../../../FlowNetwork/FordFulkerson.hpp"
#include "../../../FlowNetwork/HopcroftKarp.hpp"
#include "../../../FlowNetwork/Kuhn.hpp"
#include "../../../FlowNetwork/MPM.hpp"
#include "../../../FlowNetwork/PushRelabel.hpp"
#include "../../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>
#include <queue>
#include <vector>

using FlowSolver = Dinic;

struct InputEdge
{
	Size u, v;
	int flag;
};

void task()
{
	Size num_nodes, num_edges, source, sink;
	if (!(std::cin >> num_nodes >> num_edges >> source >> sink))
		return;

	--source;
	--sink;

	std::vector<InputEdge> input_edges(num_edges);
	const auto cut_net = FlowSolver::create(num_nodes);

	for (Size i = 0; i < num_edges; ++i)
	{
		Size u, v;
		int flag;
		std::cin >> u >> v >> flag;
		--u;
		--v;
		input_edges[i] = {u, v, flag};

		if (flag == 0)
		{
			cut_net->add_edge(u, v, INF);
		}
		else
		{
			cut_net->add_edge(u, v, 1);
			cut_net->add_edge(v, u, INF);
		}
	}

	const Long min_saturated = cut_net->compute_max_flow(source, sink);

	std::vector<bool> in_s(num_nodes, false);
	std::queue<Size> q;
	in_s[source] = true;
	q.push(source);

	const auto &cut_adj = cut_net->get_adjacency();
	const auto &cut_edges = cut_net->get_edges();

	while (!q.empty())
	{
		const Size u = q.front();
		q.pop();

		for (const Size edge_id : cut_adj[u])
		{
			const auto &e = cut_edges[edge_id];
			if (!in_s[e.to] && e.capacity - e.flow > 0)
			{
				in_s[e.to] = true;
				q.push(e.to);
			}
		}
	}

	const Size super_source = num_nodes;
	const Size super_sink = num_nodes + 1;
	const auto circ_net = FlowSolver::create(num_nodes + 2);
	std::vector<Size> circ_edge_id(num_edges, MAX);

	for (Size i = 0; i < num_edges; ++i)
	{
		if (input_edges[i].flag == 1)
		{
			const Size u = input_edges[i].u;
			const Size v = input_edges[i].v;
			circ_edge_id[i] = circ_net->get_edges().size();
			circ_net->add_edge(u, v, INF);
			circ_net->add_edge(super_source, v, 1);
			circ_net->add_edge(u, super_sink, 1);
		}
	}

	circ_net->add_edge(sink, source, INF);
	circ_net->compute_max_flow(super_source, super_sink);

	const auto &circ_edges = circ_net->get_edges();

	std::cout << min_saturated << '\n';
	for (Size i = 0; i < num_edges; ++i)
	{
		if (input_edges[i].flag == 0)
		{
			std::cout << "0 1\n";
		}
		else
		{
			const Long flow = 1 + circ_edges[circ_edge_id[i]].flow;
			const bool saturated = in_s[input_edges[i].u] && !in_s[input_edges[i].v];
			const Long cap = saturated ? flow : (flow + 1);
			std::cout << flow << ' ' << cap << '\n';
		}
	}
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

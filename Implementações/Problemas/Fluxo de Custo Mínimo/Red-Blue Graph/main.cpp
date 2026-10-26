#include "../../../CostNetwork/CycleCanceling.hpp"
#include "../../../CostNetwork/KuhnMunkres.hpp"
#include "../../../CostNetwork/NetworkSimplex.hpp"
#include "../../../CostNetwork/SuccessiveShortest.hpp"
#include "../../../CostNetwork/SuccessiveShortestDijkstra.hpp"
#include <iostream>
#include <string>
#include <vector>

using CostSolver = SuccessiveShortestDijkstra;

void add_bounded_edge(
    const std::unique_ptr<CostNetwork> &cn, std::vector<Long> &balance, const Size u,
    const Size v, const Long low, const Long high, const Long cost
)
{
	balance[u] -= low;
	balance[v] += low;
	cn->add_edge(u, v, high - low, cost);
}

void task()
{
	Size n1, n2, m;
	Long r_cost, b_cost;
	if (!(std::cin >> n1 >> n2 >> m >> r_cost >> b_cost))
		return;

	std::string s1, s2;
	std::cin >> s1 >> s2;

	const Size source = n1 + n2;
	const Size sink = n1 + n2 + 1;
	const Size super_source = n1 + n2 + 2;
	const Size super_sink = n1 + n2 + 3;

	const auto cn = CostSolver::create(n1 + n2 + 4);
	std::vector<Long> balance(n1 + n2 + 2, 0);

	std::vector<Size> red_edge_id(m);
	std::vector<Size> blue_edge_id(m);

	for (Size i = 0; i < m; ++i)
	{
		Size u, v;
		std::cin >> u >> v;
		--u;
		v = n1 + (v - 1);

		red_edge_id[i] = cn->get_edges().size();
		cn->add_edge(u, v, 1, r_cost);

		blue_edge_id[i] = cn->get_edges().size();
		cn->add_edge(v, u, 1, b_cost);
	}

	const Long inf_cap = static_cast<Long>(m) + 5;

	for (Size i = 0; i < n1; ++i)
	{
		if (s1[i] == 'R')
			add_bounded_edge(cn, balance, source, i, 1, inf_cap, 0);
		else if (s1[i] == 'B')
			add_bounded_edge(cn, balance, i, sink, 1, inf_cap, 0);
		else
		{
			add_bounded_edge(cn, balance, source, i, 0, inf_cap, 0);
			add_bounded_edge(cn, balance, i, sink, 0, inf_cap, 0);
		}
	}

	for (Size j = 0; j < n2; ++j)
	{
		const Size node = n1 + j;
		if (s2[j] == 'R')
			add_bounded_edge(cn, balance, node, sink, 1, inf_cap, 0);
		else if (s2[j] == 'B')
			add_bounded_edge(cn, balance, source, node, 1, inf_cap, 0);
		else
		{
			add_bounded_edge(cn, balance, source, node, 0, inf_cap, 0);
			add_bounded_edge(cn, balance, node, sink, 0, inf_cap, 0);
		}
	}

	cn->add_edge(sink, source, INF, 0);

	Long required_flow = 0;
	std::vector<Size> demand_edges;

	for (Size u = 0; u <= sink; ++u)
	{
		if (balance[u] > 0)
		{
			demand_edges.push_back(cn->get_edges().size());
			cn->add_edge(super_source, u, balance[u], 0);
			required_flow += balance[u];
		}
		else if (balance[u] < 0)
		{
			cn->add_edge(u, super_sink, -balance[u], 0);
		}
	}

	const Long total_cost = cn->compute_min_cost_max_flow(super_source, super_sink);
	const auto &edges = cn->get_edges();

	Long achieved_flow = 0;
	for (const Size id : demand_edges)
		achieved_flow += edges[id].flow;

	if (achieved_flow != required_flow)
	{
		std::cout << -1 << '\n';
		return;
	}

	std::string coloring(m, 'U');
	for (Size i = 0; i < m; ++i)
	{
		if (edges[red_edge_id[i]].flow > 0)
			coloring[i] = 'R';
		else if (edges[blue_edge_id[i]].flow > 0)
			coloring[i] = 'B';
	}

	std::cout << total_cost << '\n' << coloring << '\n';
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

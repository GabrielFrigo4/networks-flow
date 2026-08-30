#include "../../CostNetwork/CycleCanceling.hpp"
#include "../../CostNetwork/NetworkSimplex.hpp"
#include "../../CostNetwork/SuccessiveShortest.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using CostSolver = SuccessiveShortest;

struct EdgePair
{
	Size u, v;

	EdgePair(Size a = 0, Size b = 0)
	{
		if (a > b)
			std::swap(a, b);
		u = a;
		v = b;
	}

	bool operator==(const EdgePair &other) const
	{
		return u == other.u && v == other.v;
	}

	bool shares_endpoint(const EdgePair &other) const
	{
		return u == other.u || u == other.v || v == other.u || v == other.v;
	}
};

void task()
{
	Size num_stations, m1, m2;
	if (!(std::cin >> num_stations >> m1 >> m2))
		return;

	std::vector<EdgePair> initial_edges(m1);
	for (Size i = 0; i < m1; i++)
	{
		Size u, v;
		std::cin >> u >> v;
		initial_edges[i] = EdgePair(u, v);
	}

	std::vector<EdgePair> desired_edges(m2);
	for (Size j = 0; j < m2; j++)
	{
		Size u, v;
		std::cin >> u >> v;
		desired_edges[j] = EdgePair(u, v);
	}

	if (m1 != m2)
	{
		std::cout << -1 << "\n";
		return;
	}

	if (m1 == 0)
	{
		std::cout << 0 << "\n";
		return;
	}

	const Size m = m1;
	const Size source = 0;
	const Size dummy = 1;
	const Size total_nodes = 2 * m + 3;
	const Size sink = total_nodes - 1;

	const auto cn = CostSolver::create(total_nodes);

	for (Size i = 0; i < m; i++)
	{
		const Size init_node = 2 + i;
		cn->add_edge(source, init_node, 1, 0);
		cn->add_edge(init_node, dummy, 1, 1);
	}

	for (Size j = 0; j < m; j++)
	{
		const Size des_node = 2 + m + j;
		cn->add_edge(dummy, des_node, 1, 1);
		cn->add_edge(des_node, sink, 1, 0);
	}

	for (Size i = 0; i < m; i++)
	{
		const Size init_node = 2 + i;
		for (Size j = 0; j < m; j++)
		{
			const Size des_node = 2 + m + j;
			if (initial_edges[i] == desired_edges[j])
			{
				cn->add_edge(init_node, des_node, 1, 0);
			}
			else if (initial_edges[i].shares_endpoint(desired_edges[j]))
			{
				cn->add_edge(init_node, des_node, 1, 1);
			}
		}
	}

	const Long min_cost = cn->compute_min_cost_max_flow(source, sink);
	const Long total_flow = cn->get_total_flow(source);

	if (total_flow < static_cast<Long>(m))
	{
		std::cout << -1 << "\n";
	}
	else
	{
		std::cout << min_cost << "\n";
	}
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

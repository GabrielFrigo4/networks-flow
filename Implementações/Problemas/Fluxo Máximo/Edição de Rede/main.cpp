#include "../../../FlowNetwork/Dinic.hpp"
#include "../../../FlowNetwork/EdmondsKarp.hpp"
#include "../../../FlowNetwork/FordFulkerson.hpp"
#include "../../../FlowNetwork/PushRelabel.hpp"
#include "../../../FlowNetwork/PushRelabelImproved.hpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using FlowSolver = PushRelabelImproved;

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

	// 1. Remover gulosamente arestas idênticas (Custo 0)
	std::vector<bool> used_init(m1, false);
	std::vector<bool> used_des(m2, false);

	for (Size i = 0; i < m1; i++)
	{
		for (Size j = 0; j < m2; j++)
		{
			if (!used_des[j] && initial_edges[i] == desired_edges[j])
			{
				used_init[i] = true;
				used_des[j] = true;
				break;
			}
		}
	}

	std::vector<EdgePair> rem_init;
	std::vector<EdgePair> rem_des;

	for (Size i = 0; i < m1; i++)
	{
		if (!used_init[i])
			rem_init.push_back(initial_edges[i]);
	}
	for (Size j = 0; j < m2; j++)
	{
		if (!used_des[j])
			rem_des.push_back(desired_edges[j]);
	}

	const Size m_rem = rem_init.size();
	if (m_rem == 0)
	{
		std::cout << 0 << "\n";
		return;
	}

	// 2. Construir Grafo Bipartido de Emparelhamento Máximo para arestas que compartilham 1 ponta (Custo 1)
	const Size total_nodes = 2 * m_rem + 2;
	const Size source = 0;
	const Size sink = total_nodes - 1;

	const auto fn = FlowSolver::create(total_nodes);

	for (Size i = 0; i < m_rem; i++)
	{
		fn->add_edge(source, 1 + i, 1);
	}

	for (Size j = 0; j < m_rem; j++)
	{
		fn->add_edge(1 + m_rem + j, sink, 1);
	}

	for (Size i = 0; i < m_rem; i++)
	{
		for (Size j = 0; j < m_rem; j++)
		{
			if (rem_init[i].shares_endpoint(rem_des[j]))
			{
				fn->add_edge(1 + i, 1 + m_rem + j, 1);
			}
		}
	}

	// 3. Emparelhamento Máximo F
	const Long matching = fn->compute_max_flow(source, sink);

	// 4. Fórmula Fechada: Custo = 1 * matching + 2 * (m_rem - matching) = 2 * m_rem - matching
	const Long total_cost = 2 * static_cast<Long>(m_rem) - matching;

	std::cout << total_cost << "\n";
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

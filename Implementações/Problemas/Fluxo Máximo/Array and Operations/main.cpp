#include "../../../FlowNetwork/Dinic.hpp"
#include "../../../FlowNetwork/EdmondsKarp.hpp"
#include "../../../FlowNetwork/FordFulkerson.hpp"
#include "../../../FlowNetwork/HopcroftKarp.hpp"
#include "../../../FlowNetwork/Kuhn.hpp"
#include "../../../FlowNetwork/MPM.hpp"
#include "../../../FlowNetwork/PushRelabel.hpp"
#include "../../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>
#include <utility>
#include <vector>

using FlowSolver = HopcroftKarp;

struct FactorGroup
{
	Long prime;
	std::vector<Size> node_ids;
};

void task()
{
	Size n, m;
	if (!(std::cin >> n >> m))
		return;

	std::vector<Long> values(n + 1);
	for (Size i = 1; i <= n; ++i)
		std::cin >> values[i];

	std::vector<std::pair<Size, Size>> good_pairs(m);
	for (Size k = 0; k < m; ++k)
	{
		Size u, v;
		std::cin >> u >> v;
		if ((u & 1U) == 0U)
			std::swap(u, v);
		good_pairs[k] = {u, v};
	}

	std::vector<std::vector<FactorGroup>> factors(n + 1);
	Size next_node = 1;

	for (Size i = 1; i <= n; ++i)
	{
		Long x = values[i];
		for (Long d = 2; d * d <= x; ++d)
		{
			if (x % d != 0)
				continue;

			FactorGroup group{d, {}};
			while (x % d == 0)
			{
				group.node_ids.push_back(next_node++);
				x /= d;
			}
			factors[i].push_back(std::move(group));
		}
		if (x > 1)
		{
			FactorGroup group{x, {next_node++}};
			factors[i].push_back(std::move(group));
		}
	}

	const Size source = 0;
	const Size sink = next_node;
	const auto fn = FlowSolver::create(sink + 1);

	for (Size i = 1; i <= n; ++i)
	{
		for (const auto &group : factors[i])
		{
			for (const Size node_id : group.node_ids)
			{
				if ((i & 1U) != 0U)
					fn->add_edge(source, node_id, 1);
				else
					fn->add_edge(node_id, sink, 1);
			}
		}
	}

	for (const auto &[u, v] : good_pairs)
	{
		for (const auto &gu : factors[u])
		{
			for (const auto &gv : factors[v])
			{
				if (gu.prime != gv.prime)
					continue;

				for (const Size nu : gu.node_ids)
				{
					for (const Size nv : gv.node_ids)
						fn->add_edge(nu, nv, 1);
				}
			}
		}
	}

	std::cout << fn->compute_max_flow(source, sink) << '\n';
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}

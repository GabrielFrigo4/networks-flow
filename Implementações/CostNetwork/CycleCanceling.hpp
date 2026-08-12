#ifndef CYCLE_CANCELING_HPP
#define CYCLE_CANCELING_HPP

#include "CostNetwork.hpp"
#include <algorithm>
#include <queue>

class CycleCanceling : public CostNetwork
{
public:
	explicit CycleCanceling(const Size n) : CostNetwork(n), distance(n), parent_edge(n) {}

	static std::unique_ptr<CostNetwork> create(const Size n)
	{
		return std::make_unique<CycleCanceling>(n);
	}

	std::unique_ptr<CostNetwork> make(const Size n) const override
	{
		return std::make_unique<CycleCanceling>(n);
	}

	std::unique_ptr<CostNetwork> clone() const override
	{
		return std::make_unique<CycleCanceling>(*this);
	}

	Long compute_min_cost_max_flow(const Size source, const Size sink) override
	{
		saturate_max_flow(source, sink);

		while (find_negative_cycle())
			cancel_cycle();

		Long total_cost = 0;
		for (Size i = 0; i < edges.size(); i += 2)
			total_cost += edges[i].flow * edges[i].cost;
		return total_cost;
	}

private:
	std::vector<Long> distance;
	std::vector<Size> parent_edge;
	Size cycle_node;

	void saturate_max_flow(const Size source, const Size sink)
	{
		while (true)
		{
			std::vector<Size> parent(size, MAX);
			std::queue<std::pair<Size, Long>> queue;

			parent[source] = source;
			queue.push({source, INF});

			while (!queue.empty())
			{
				const auto [current, current_flow] = queue.front();
				queue.pop();

				for (const Size edge_id : adjacency[current])
				{
					const Size next = edges[edge_id].to;
					if (parent[next] != MAX || get_residual_capacity(edge_id) <= 0)
						continue;

					parent[next] = edge_id;
					const Long bottleneck = std::min(current_flow, get_residual_capacity(edge_id));

					if (next == sink)
					{
						Size node = sink;
						while (node != source)
						{
							const Size eid = parent[node];
							push_flow(eid, bottleneck);
							node = edges[eid].from;
						}
						goto next_iteration;
					}
					queue.push({next, bottleneck});
				}
			}
			return;
			next_iteration:;
		}
	}

	bool find_negative_cycle()
	{
		std::fill(distance.begin(), distance.end(), 0);
		std::fill(parent_edge.begin(), parent_edge.end(), MAX);

		for (Size iteration = 0; iteration < size; ++iteration)
		{
			cycle_node = MAX;
			for (Size edge_id = 0; edge_id < edges.size(); ++edge_id)
			{
				if (get_residual_capacity(edge_id) <= 0)
					continue;

				const Size u = edges[edge_id].from;
				const Size v = edges[edge_id].to;

				if (distance[u] + edges[edge_id].cost < distance[v])
				{
					distance[v] = distance[u] + edges[edge_id].cost;
					parent_edge[v] = edge_id;
					cycle_node = v;
				}
			}
		}
		return cycle_node != MAX;
	}

	void cancel_cycle()
	{
		Size node = cycle_node;
		for (Size i = 0; i < size; ++i)
			node = edges[parent_edge[node]].from;

		Long bottleneck = INF;
		Size current = node;
		do
		{
			const Size edge_id = parent_edge[current];
			bottleneck = std::min(bottleneck, get_residual_capacity(edge_id));
			current = edges[edge_id].from;
		} while (current != node);

		current = node;
		do
		{
			const Size edge_id = parent_edge[current];
			push_flow(edge_id, bottleneck);
			current = edges[edge_id].from;
		} while (current != node);
	}
};

#endif // CYCLE_CANCELING_HPP

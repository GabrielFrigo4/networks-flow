#ifndef EDMONDS_KARP_HPP
#define EDMONDS_KARP_HPP

#include "FlowNetwork.hpp"
#include <queue>

class EdmondsKarp : public FlowNetwork
{
public:
	explicit EdmondsKarp(const Size n) : FlowNetwork(n), parent_edge(n, MAX) {}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<EdmondsKarp>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<EdmondsKarp>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<EdmondsKarp>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		Long total_flow = 0;

		while (const Long bottleneck = bfs(source, sink))
		{
			total_flow += bottleneck;
			retrace(source, sink, bottleneck);
		}

		return total_flow;
	}

private:
	std::vector<Size> parent_edge;

	Long bfs(const Size source, const Size sink)
	{
		std::fill(parent_edge.begin(), parent_edge.end(), MAX);
		std::queue<std::pair<Size, Long>> queue;

		parent_edge[source] = source;
		queue.push({source, INF});

		while (!queue.empty())
		{
			const auto [current_node, current_flow] = queue.front();
			queue.pop();

			for (const Size edge_id : adjacency[current_node])
			{
				const Size next_node = edges[edge_id].to;
				const Long residual_capacity = get_residual_capacity(edge_id);

				if (parent_edge[next_node] != MAX || residual_capacity <= 0)
					continue;

				parent_edge[next_node] = edge_id;
				const Long bottleneck = std::min(current_flow, residual_capacity);

				if (next_node == sink)
					return bottleneck;

				queue.push({next_node, bottleneck});
			}
		}
		return 0;
	}

	void retrace(const Size source, const Size sink, const Long bottleneck)
	{
		Size current_node = sink;
		while (current_node != source)
		{
			const Size edge_id = parent_edge[current_node];
			push_flow(edge_id, bottleneck);
			current_node = edges[edge_id].from;
		}
	}
};

#endif // EDMONDS_KARP_HPP

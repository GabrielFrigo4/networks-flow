#ifndef EDMONDS_KARP_HPP
#define EDMONDS_KARP_HPP

#include "FlowNetwork.hpp"
#include <queue>

class EdmondsKarp : public FlowNetwork
{
public:
	explicit EdmondsKarp(const Size n) : FlowNetwork(n) {}

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
		Long new_flow = 0;
		std::vector<Size> parent_edge(size);

		while ((new_flow = bfs(source, sink, parent_edge)) > 0)
		{
			total_flow += new_flow;
			update_path_flow(source, sink, parent_edge, new_flow);
		}

		return total_flow;
	}

private:
	Long bfs(const Size source, const Size sink, std::vector<Size> &parent_edge)
	{
		std::fill(parent_edge.begin(), parent_edge.end(), MAX);
		std::queue<std::pair<Size, Long>> queue;

		parent_edge[source] = source;
		queue.push({source, INF});

		while (!queue.empty())
		{
			const auto [current_node, current_flow] = queue.front();
			queue.pop();

			for (const Size edge_id : adj[current_node])
			{
				const Size next_node = edges[edge_id].to;
				const Long residual_capacity = get_residual_capacity(edge_id);

				if (parent_edge[next_node] != MAX || residual_capacity <= 0)
					continue;

				parent_edge[next_node] = edge_id;
				const Long pushed_flow = std::min(current_flow, residual_capacity);

				if (next_node == sink)
					return pushed_flow;

				queue.push({next_node, pushed_flow});
			}
		}
		return 0;
	}

	void update_path_flow(
	    const Size source, const Size sink, const std::vector<Size> &parent_edge,
	    const Long new_flow
	)
	{
		Size current_node = sink;
		while (current_node != source)
		{
			const Size edge_id = parent_edge[current_node];
			push_flow(edge_id, new_flow);
			current_node = edges[edge_id].from;
		}
	}
};

#endif // EDMONDS_KARP_HPP

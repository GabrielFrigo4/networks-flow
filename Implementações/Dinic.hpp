#ifndef DINIC_HPP
#define DINIC_HPP

#include "FlowNetwork.hpp"
#include <queue>

class Dinic : public FlowNetwork
{
public:
	explicit Dinic(const Size n) : FlowNetwork(n), level(n), next_edge_ptr(n) {}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<Dinic>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<Dinic>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<Dinic>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		Long total_flow = 0;
		while (bfs(source, sink))
		{
			std::fill(next_edge_ptr.begin(), next_edge_ptr.end(), 0);
			while (const Long flow_pushed = dfs(source, sink, INF))
				total_flow += flow_pushed;
		}
		return total_flow;
	}

private:
	std::vector<Size> level;
	std::vector<Size> next_edge_ptr;

	bool bfs(const Size source, const Size sink)
	{
		std::fill(level.begin(), level.end(), MAX);
		std::queue<Size> queue;

		level[source] = 0;
		queue.push(source);

		while (!queue.empty())
		{
			const Size current_node = queue.front();
			queue.pop();

			for (const Size edge_id : adjacency[current_node])
			{
				const Size next_node = edges[edge_id].to;
				const Long residual_capacity = get_residual_capacity(edge_id);

				if (level[next_node] != MAX || residual_capacity <= 0)
					continue;

				level[next_node] = level[current_node] + 1;
				queue.push(next_node);
			}
		}
		return level[sink] != MAX;
	}

	Long dfs(const Size current_node, const Size sink, const Long flow_pushed)
	{
		if (current_node == sink)
			return flow_pushed;

		for (Size &ptr = next_edge_ptr[current_node]; ptr < adjacency[current_node].size();
		     ++ptr)
		{
			const Size edge_id = adjacency[current_node][ptr];
			const Size next_node = edges[edge_id].to;
			const Long residual_capacity = get_residual_capacity(edge_id);

			if (level[current_node] + 1 != level[next_node] ||
			    residual_capacity <= 0)
				continue;

			const Long bottleneck = std::min(flow_pushed, residual_capacity);
			const Long flow_transmitted = dfs(next_node, sink, bottleneck);

			if (flow_transmitted == 0)
				continue;

			push_flow(edge_id, flow_transmitted);
			return flow_transmitted;
		}
		return 0;
	}
};

#endif // DINIC_HPP

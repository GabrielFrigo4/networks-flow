#ifndef FORD_FULKERSON_HPP
#define FORD_FULKERSON_HPP

#include "FlowNetwork.hpp"

class FordFulkerson : public FlowNetwork
{
public:
	explicit FordFulkerson(const Size n) : FlowNetwork(n) {}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<FordFulkerson>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<FordFulkerson>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<FordFulkerson>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		Long total_flow = 0;
		std::vector<bool> visited(size, false);

		while (true)
		{
			std::fill(visited.begin(), visited.end(), false);
			const Long new_flow = dfs(source, sink, INF, visited);
			if (new_flow == 0)
				break;
			total_flow += new_flow;
		}

		return total_flow;
	}

private:
	Long dfs(
	    const Size current_node, const Size sink, const Long flow_pushed,
	    std::vector<bool> &visited
	)
	{
		if (current_node == sink)
			return flow_pushed;

		visited[current_node] = true;

		for (const Size edge_id : adj[current_node])
		{
			const Size next_node = edges[edge_id].to;
			const Long residual_capacity = get_residual_capacity(edge_id);

			if (visited[next_node] || residual_capacity <= 0)
				continue;

			const Long bottleneck = std::min(flow_pushed, residual_capacity);
			const Long flow_transmitted = dfs(next_node, sink, bottleneck, visited);

			if (flow_transmitted > 0)
			{
				push_flow(edge_id, flow_transmitted);
				return flow_transmitted;
			}
		}

		return 0;
	}
};

#endif // FORD_FULKERSON_HPP

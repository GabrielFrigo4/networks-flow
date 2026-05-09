# [Dinic's Algorithm](https://cp-algorithms.com/graph/dinic.html)

## CSES

1.  [Download Speed](https://cses.fi/problemset/task/1694)
2.  [School Dance](https://cses.fi/problemset/task/1696)
3.  [Distinct Routes](https://cses.fi/problemset/task/1711)
4.  [Police Chase](https://cses.fi/problemset/task/1695)

## Beecrowd

1.  [Time Travel](https://judge.beecrowd.com/en/problems/view/2082)

## FlowNetwork

```cpp
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <vector>

using Long = long long;
using Size = std::size_t;

constexpr Long INF = std::numeric_limits<Long>::max() >> 8;
constexpr Size MAX = std::numeric_limits<Size>::max() >> 8;

class FlowNetwork
{
public:
	struct Edge
	{
		Size from, to;
		Long capacity, flow;
	};

	explicit FlowNetwork(const Size n) : size(n), adj(n) {}
	virtual ~FlowNetwork() = default;

	virtual std::unique_ptr<FlowNetwork> make(const Size n) const = 0;
	virtual std::unique_ptr<FlowNetwork> clone() const = 0;

	virtual void add_edge(
	    const Size from, const Size to, const Long capacity,
	    const Long reverse_capacity = 0
	)
	{
		adj[from].push_back(edges.size());
		edges.push_back({from, to, capacity, 0});
		adj[to].push_back(edges.size());
		edges.push_back({to, from, reverse_capacity, 0});
	}

	virtual Long compute_max_flow(const Size source, const Size sink) = 0;

	[[nodiscard]] Size get_size() const
	{
		return size;
	}

	[[nodiscard]] const std::vector<Edge> &get_edges() const
	{
		return edges;
	}

	[[nodiscard]] const std::vector<std::vector<Size>> &get_adj() const
	{
		return adj;
	}

protected:
	Size size;
	std::vector<Edge> edges;
	std::vector<std::vector<Size>> adj;

	[[nodiscard]] Long get_residual_capacity(const Size edge_id) const
	{
		return edges[edge_id].capacity - edges[edge_id].flow;
	}

	void push_flow(const Size edge_id, const Long flow_amount)
	{
		edges[edge_id].flow += flow_amount;
		edges[edge_id ^ 1ULL].flow -= flow_amount;
	}
};

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
			{
				total_flow += flow_pushed;
			}
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

			for (const Size edge_id : adj[current_node])
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
		if (flow_pushed == 0 || current_node == sink)
			return flow_pushed;

		for (Size &ptr = next_edge_ptr[current_node]; ptr < adj[current_node].size();
		     ++ptr)
		{
			const Size edge_id = adj[current_node][ptr];
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
```

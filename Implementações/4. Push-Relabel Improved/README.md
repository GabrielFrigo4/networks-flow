# [Push-Relabel Improved](https://cp-algorithms.com/graph/push-relabel-faster.html)

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

class PushRelabelImproved : public FlowNetwork
{
public:
	explicit PushRelabelImproved(const Size n) : FlowNetwork(n) {}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<PushRelabelImproved>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<PushRelabelImproved>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<PushRelabelImproved>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		initialize_preflow(source, sink);

		while (true)
		{
			if (active_buckets[highest_active].empty())
			{
				if (highest_active == 0)
					break;
				highest_active--;
				continue;
			}

			const Size current_node = active_buckets[highest_active].back();
			active_buckets[highest_active].pop_back();

			if (height[current_node] != highest_active)
			{
				continue;
			}

			discharge_node(current_node, source, sink);
		}

		return excess[sink];
	}

private:
	std::vector<Size> height;
	std::vector<Long> excess;
	std::vector<Size> height_count;
	std::vector<Size> next_edge_ptr;
	std::vector<std::vector<Size>> active_buckets;
	Size highest_active;

	void initialize_preflow(const Size source, const Size sink)
	{
		const Size max_height_limit = size * 2 + 1;

		height.assign(size, 0);
		excess.assign(size, 0);
		height_count.assign(max_height_limit, 0);
		next_edge_ptr.assign(size, 0);
		active_buckets.assign(max_height_limit, std::vector<Size>());
		highest_active = 0;

		height[source] = size;
		height_count[size] = 1;
		height_count[0] = size - 1;

		excess[source] = INF;
		for (const Size edge_id : adj[source])
		{
			push_preflow(source, edge_id, source, sink);
		}
	}

	void discharge_node(const Size current_node, const Size source, const Size sink)
	{
		while (excess[current_node] > 0)
		{
			if (next_edge_ptr[current_node] >= adj[current_node].size())
			{
				relabel_node(current_node, source, sink);
				next_edge_ptr[current_node] = 0;
				continue;
			}

			const Size edge_id = adj[current_node][next_edge_ptr[current_node]];
			const Size next_node = edges[edge_id].to;
			const Long residual_capacity = get_residual_capacity(edge_id);

			if (residual_capacity > 0 &&
			    height[current_node] == height[next_node] + 1)
			{
				push_preflow(current_node, edge_id, source, sink);
			}
			else
			{
				next_edge_ptr[current_node]++;
			}
		}
	}

	void relabel_node(const Size current_node, const Size source, const Size sink)
	{
		Size min_height = MAX;
		for (const Size edge_id : adj[current_node])
		{
			if (get_residual_capacity(edge_id) <= 0)
			{
				continue;
			}

			min_height = std::min(min_height, height[edges[edge_id].to]);
		}

		if (min_height == MAX)
		{
			return;
		}

		const Size old_height = height[current_node];
		height_count[old_height]--;

		height[current_node] = min_height + 1;
		height_count[height[current_node]]++;
		enqueue_active_node(current_node, source, sink);

		if (height_count[old_height] == 0 && old_height < size)
		{
			apply_gap_heuristic(old_height, source, sink);
		}
	}

	void apply_gap_heuristic(
	    const Size gap_height, const Size source, const Size sink
	)
	{
		for (Size i = 0; i < size; ++i)
		{
			if (i == source || height[i] <= gap_height || height[i] >= size)
			{
				continue;
			}

			height_count[height[i]]--;
			height[i] = std::max(height[i], size + 1);
			height_count[height[i]]++;
			enqueue_active_node(i, source, sink);
		}
	}

	void push_preflow(
	    const Size current_node, const Size edge_id, const Size source,
	    const Size sink
	)
	{
		const Size next_node = edges[edge_id].to;
		const Long residual_capacity = get_residual_capacity(edge_id);

		if (residual_capacity == 0 || height[current_node] <= height[next_node])
		{
			return;
		}

		const Long flow_to_push = std::min(excess[current_node], residual_capacity);
		push_flow(edge_id, flow_to_push);
		excess[current_node] -= flow_to_push;
		excess[next_node] += flow_to_push;

		if (excess[next_node] == flow_to_push)
		{
			enqueue_active_node(next_node, source, sink);
		}
	}

	void enqueue_active_node(const Size node, const Size source, const Size sink)
	{
		if (node == source || node == sink || excess[node] <= 0)
		{
			return;
		}

		active_buckets[height[node]].push_back(node);
		highest_active = std::max(highest_active, height[node]);
	}
};
```

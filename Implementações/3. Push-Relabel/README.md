# [Push-Relabel](https://cp-algorithms.com/graph/push-relabel.html)

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

class PushRelabel : public FlowNetwork
{
public:
	explicit PushRelabel(const Size n)
	    : FlowNetwork(n), height(n), excess(n), next_edge_ptr(n)
	{
	}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<PushRelabel>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<PushRelabel>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<PushRelabel>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		std::queue<Size> active_queue;
		std::vector<bool> in_queue(size, false);

		initialize_preflow(source, sink, active_queue, in_queue);

		while (!active_queue.empty())
		{
			const Size current_node = active_queue.front();
			active_queue.pop();
			in_queue[current_node] = false;
			discharge_node(current_node, active_queue, in_queue, source, sink);
		}

		return excess[sink];
	}

private:
	std::vector<Size> height;
	std::vector<Long> excess;
	std::vector<Size> next_edge_ptr;

	void initialize_preflow(
	    const Size source, const Size sink, std::queue<Size> &active_queue,
	    std::vector<bool> &in_queue
	)
	{
		std::fill(height.begin(), height.end(), 0);
		std::fill(excess.begin(), excess.end(), 0);
		std::fill(next_edge_ptr.begin(), next_edge_ptr.end(), 0);

		height[source] = size;
		excess[source] = INF;

		for (const Size edge_id : adj[source])
		{
			if (get_residual_capacity(edge_id) <= 0)
			{
				continue;
			}

			push_preflow(source, edge_id);
			const Size next_node = edges[edge_id].to;

			if (next_node == source || next_node == sink || in_queue[next_node])
			{
				continue;
			}

			active_queue.push(next_node);
			in_queue[next_node] = true;
		}
	}

	void discharge_node(
	    const Size current_node, std::queue<Size> &active_queue,
	    std::vector<bool> &in_queue, const Size source, const Size sink
	)
	{
		while (excess[current_node] > 0)
		{
			if (next_edge_ptr[current_node] >= adj[current_node].size())
			{
				relabel_node(current_node);
				next_edge_ptr[current_node] = 0;
				continue;
			}

			const Size edge_id = adj[current_node][next_edge_ptr[current_node]];
			const Size next_node = edges[edge_id].to;
			const Long residual_capacity = get_residual_capacity(edge_id);

			if (residual_capacity > 0 &&
			    height[current_node] == height[next_node] + 1)
			{
				push_preflow(current_node, edge_id);

				if (next_node != source && next_node != sink &&
				    !in_queue[next_node] && excess[next_node] > 0)
				{
					active_queue.push(next_node);
					in_queue[next_node] = true;
				}
			}
			else
			{
				next_edge_ptr[current_node]++;
			}
		}
	}

	void relabel_node(const Size current_node)
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

		if (min_height != MAX)
		{
			height[current_node] = min_height + 1;
		}
	}

	void push_preflow(const Size current_node, const Size edge_id)
	{
		const Size next_node = edges[edge_id].to;
		const Long residual_capacity = get_residual_capacity(edge_id);
		const Long flow_to_push = std::min(excess[current_node], residual_capacity);

		if (flow_to_push == 0)
			return;

		push_flow(edge_id, flow_to_push);
		excess[current_node] -= flow_to_push;
		excess[next_node] += flow_to_push;
	}
};
```

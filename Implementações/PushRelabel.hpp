#ifndef PUSH_RELABEL_HPP
#define PUSH_RELABEL_HPP

#include "FlowNetwork.hpp"
#include <queue>

class PushRelabel : public FlowNetwork
{
public:
	explicit PushRelabel(const Size n)
	    : FlowNetwork(n), height(n), excess(n), next_edge_ptr(n), in_queue(n, false),
	      source_node(0), sink_node(0)
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
		source_node = source;
		sink_node = sink;
		initialize_preflow();

		while (!active_queue.empty())
		{
			const Size current_node = active_queue.front();
			active_queue.pop();
			in_queue[current_node] = false;
			discharge_node(current_node);
		}

		return excess[sink];
	}

private:
	std::vector<Size> height;
	std::vector<Long> excess;
	std::vector<Size> next_edge_ptr;
	std::queue<Size> active_queue;
	std::vector<bool> in_queue;
	Size source_node, sink_node;

	void initialize_preflow()
	{
		std::fill(height.begin(), height.end(), 0);
		std::fill(excess.begin(), excess.end(), 0);
		std::fill(next_edge_ptr.begin(), next_edge_ptr.end(), 0);
		std::fill(in_queue.begin(), in_queue.end(), false);
		active_queue = {};

		height[source_node] = size;

		for (const Size edge_id : adjacency[source_node])
		{
			const Long cap = edges[edge_id].capacity;
			if (cap <= 0)
				continue;

			push_flow(edge_id, cap);
			const Size next_node = edges[edge_id].to;
			excess[next_node] += cap;

			if (next_node == source_node || next_node == sink_node ||
			    in_queue[next_node])
				continue;

			active_queue.push(next_node);
			in_queue[next_node] = true;
		}
	}

	void discharge_node(const Size current_node)
	{
		while (excess[current_node] > 0)
		{
			if (next_edge_ptr[current_node] >= adjacency[current_node].size())
			{
				relabel_node(current_node);
				next_edge_ptr[current_node] = 0;
				continue;
			}

			const Size edge_id = adjacency[current_node][next_edge_ptr[current_node]];
			const Size next_node = edges[edge_id].to;
			const Long residual_capacity = get_residual_capacity(edge_id);

			if (residual_capacity > 0 &&
			    height[current_node] == height[next_node] + 1)
			{
				push_preflow(current_node, edge_id);

				if (next_node != source_node && next_node != sink_node &&
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
		for (const Size edge_id : adjacency[current_node])
		{
			if (get_residual_capacity(edge_id) <= 0)
				continue;
			min_height = std::min(min_height, height[edges[edge_id].to]);
		}

		if (min_height != MAX)
			height[current_node] = min_height + 1;
	}

	void push_preflow(const Size current_node, const Size edge_id)
	{
		const Size next_node = edges[edge_id].to;
		const Long residual_capacity = get_residual_capacity(edge_id);
		const Long flow_to_push = std::min(excess[current_node], residual_capacity);

		push_flow(edge_id, flow_to_push);
		excess[current_node] -= flow_to_push;
		excess[next_node] += flow_to_push;
	}
};

#endif // PUSH_RELABEL_HPP

#ifndef FLOW_NETWORK_HPP
#define FLOW_NETWORK_HPP

#include <limits>
#include <memory>
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

	explicit FlowNetwork(const Size n) : size(n), adjacency(n) {}
	virtual ~FlowNetwork() = default;

	virtual std::unique_ptr<FlowNetwork> make(const Size n) const = 0;
	virtual std::unique_ptr<FlowNetwork> clone() const = 0;

	virtual void add_edge(
	    const Size from, const Size to, const Long capacity,
	    const Long reverse_capacity = 0
	)
	{
		adjacency[from].push_back(edges.size());
		edges.push_back({from, to, capacity, 0});
		adjacency[to].push_back(edges.size());
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

	[[nodiscard]] const std::vector<std::vector<Size>> &get_adjacency() const
	{
		return adjacency;
	}

protected:
	Size size;
	std::vector<Edge> edges;
	std::vector<std::vector<Size>> adjacency;

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

#endif // FLOW_NETWORK_HPP

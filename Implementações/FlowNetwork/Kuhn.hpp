#ifndef KUHN_HPP
#define KUHN_HPP

#include "FlowNetwork.hpp"
#include <algorithm>
#include <vector>

class Kuhn : public FlowNetwork
{
public:
	explicit Kuhn(const Size n)
	    : FlowNetwork(n), match_left(n, MAX), match_right(n, MAX),
	      matched_edge_left(n, MAX), source_edge(n, MAX), sink_edge(n, MAX),
	      bipartite_adj(n), visited(n, 0), token(0)
	{
	}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<Kuhn>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<Kuhn>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<Kuhn>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		std::vector<Size> left_nodes;
		if (!extract_bipartite_graph(source, sink, left_nodes))
			return compute_fallback_flow(source, sink);

		std::fill(match_left.begin(), match_left.end(), MAX);
		std::fill(match_right.begin(), match_right.end(), MAX);
		std::fill(matched_edge_left.begin(), matched_edge_left.end(), MAX);
		std::fill(visited.begin(), visited.end(), 0);
		token = 0;

		Long total_matched = 0;
		for (const Size u : left_nodes)
		{
			++token;
			if (dfs_augment(u))
				++total_matched;
		}

		for (const Size u : left_nodes)
		{
			const Size v = match_left[u];
			if (v == MAX)
				continue;

			push_flow(source_edge[u], 1);
			push_flow(matched_edge_left[u], 1);
			push_flow(sink_edge[v], 1);
		}

		return total_matched;
	}

private:
	std::vector<Size> match_left;
	std::vector<Size> match_right;
	std::vector<Size> matched_edge_left;
	std::vector<Size> source_edge;
	std::vector<Size> sink_edge;
	std::vector<std::vector<Size>> bipartite_adj;
	std::vector<Size> visited;
	Size token;

	bool dfs_augment(const Size u)
	{
		visited[u] = token;

		for (const Size edge_id : bipartite_adj[u])
		{
			const Size v = edges[edge_id].to;
			const Size next_u = match_right[v];

			if (next_u == MAX || (visited[next_u] != token && dfs_augment(next_u)))
			{
				match_left[u] = v;
				match_right[v] = u;
				matched_edge_left[u] = edge_id;
				return true;
			}
		}

		return false;
	}

	bool extract_bipartite_graph(
	    const Size source, const Size sink, std::vector<Size> &left_nodes
	)
	{
		std::fill(source_edge.begin(), source_edge.end(), MAX);
		std::fill(sink_edge.begin(), sink_edge.end(), MAX);
		for (Size u = 0; u < size; ++u)
			bipartite_adj[u].clear();

		std::vector<int> side(size, 0);

		for (Size id = 0; id < edges.size(); id += 2)
		{
			if (edges[id].flow != 0)
				return false;
		}

		for (const Size edge_id : adjacency[source])
		{
			if ((edge_id & 1U) != 0U)
				continue;

			const Size u = edges[edge_id].to;
			if (u == source || u == sink || edges[edge_id].capacity != 1 ||
			    source_edge[u] != MAX)
				return false;

			source_edge[u] = edge_id;
			side[u] = 1;
			left_nodes.push_back(u);
		}

		for (Size id = 0; id < edges.size(); id += 2)
		{
			const Size u = edges[id].from;
			const Size v = edges[id].to;
			const Long cap = edges[id].capacity;

			if (cap <= 0 || u == source)
				continue;

			if (v == sink)
			{
				if (u == sink || side[u] == 1 || cap != 1 || sink_edge[u] != MAX)
					return false;
				sink_edge[u] = id;
				side[u] = 2;
			}
		}

		for (Size id = 0; id < edges.size(); id += 2)
		{
			const Size u = edges[id].from;
			const Size v = edges[id].to;
			const Long cap = edges[id].capacity;

			if (cap <= 0 || u == source || v == sink)
				continue;

			if (side[u] != 1 || side[v] != 2)
				return false;

			bipartite_adj[u].push_back(id);
		}

		return true;
	}

	Long dfs_fallback(const Size u, const Size sink, const Long pushed)
	{
		if (u == sink)
			return pushed;

		visited[u] = token;

		for (const Size edge_id : adjacency[u])
		{
			const Size v = edges[edge_id].to;
			const Long cap = get_residual_capacity(edge_id);

			if (visited[v] == token || cap <= 0)
				continue;

			const Long tr = dfs_fallback(v, sink, std::min(pushed, cap));
			if (tr > 0)
			{
				push_flow(edge_id, tr);
				return tr;
			}
		}

		return 0;
	}

	Long compute_fallback_flow(const Size source, const Size sink)
	{
		std::fill(visited.begin(), visited.end(), 0);
		token = 0;
		Long total_flow = 0;

		while (true)
		{
			++token;
			const Long pushed = dfs_fallback(source, sink, INF);
			if (pushed == 0)
				break;
			total_flow += pushed;
		}

		return total_flow;
	}
};

#endif // KUHN_HPP

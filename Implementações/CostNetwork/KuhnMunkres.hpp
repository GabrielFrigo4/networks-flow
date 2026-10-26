#ifndef KUHN_MUNKRES_HPP
#define KUHN_MUNKRES_HPP

#include "CostNetwork.hpp"
#include <algorithm>
#include <queue>
#include <vector>

class KuhnMunkres : public CostNetwork
{
public:
	explicit KuhnMunkres(const Size n)
	    : CostNetwork(n), source_edge(n, MAX), sink_edge(n, MAX), left_index(n, MAX),
	      right_index(n, MAX)
	{
	}

	static std::unique_ptr<CostNetwork> create(const Size n)
	{
		return std::make_unique<KuhnMunkres>(n);
	}

	std::unique_ptr<CostNetwork> make(const Size n) const override
	{
		return std::make_unique<KuhnMunkres>(n);
	}

	std::unique_ptr<CostNetwork> clone() const override
	{
		return std::make_unique<KuhnMunkres>(*this);
	}

	Long compute_min_cost_max_flow(const Size source, const Size sink) override
	{
		std::vector<Size> left_nodes;
		std::vector<Size> right_nodes;
		std::vector<std::vector<Long>> cost_matrix;
		std::vector<std::vector<Size>> edge_matrix;

		if (!extract_assignment_matrix(
		        source, sink, left_nodes, right_nodes, cost_matrix, edge_matrix
		    ))
		{
			return compute_fallback_min_cost_flow(source, sink);
		}

		const Size n_rows = left_nodes.size();
		const Size n_cols = right_nodes.size();

		if (n_rows == 0 || n_cols == 0)
			return 0;

		std::vector<Long> u_pot(n_rows + 1, 0);
		std::vector<Long> v_pot(n_cols + 1, 0);
		std::vector<Size> match_col(n_cols + 1, 0);
		std::vector<Size> way(n_cols + 1, 0);

		for (Size i = 1; i <= n_rows; ++i)
		{
			match_col[0] = i;
			Size j0 = 0;
			std::vector<Long> minv(n_cols + 1, INF);
			std::vector<bool> used(n_cols + 1, false);

			do
			{
				used[j0] = true;
				const Size i0 = match_col[j0];
				Long delta = INF;
				Size j1 = 0;

				for (Size j = 1; j <= n_cols; ++j)
				{
					if (used[j])
						continue;

					const Long cell_cost = cost_matrix[i0 - 1][j - 1];
					if (cell_cost >= INF)
						continue;

					const Long cur = cell_cost - u_pot[i0] - v_pot[j];
					if (cur < minv[j])
					{
						minv[j] = cur;
						way[j] = j0;
					}
					if (minv[j] < delta)
					{
						delta = minv[j];
						j1 = j;
					}
				}

				if (delta == INF)
					return compute_fallback_min_cost_flow(source, sink);

				for (Size j = 0; j <= n_cols; ++j)
				{
					if (used[j])
					{
						u_pot[match_col[j]] += delta;
						v_pot[j] -= delta;
					}
					else
					{
						minv[j] -= delta;
					}
				}

				j0 = j1;
			} while (match_col[j0] != 0);

			do
			{
				const Size j1 = way[j0];
				match_col[j0] = match_col[j1];
				j0 = j1;
			} while (j0 != 0);
		}

		for (Size j = 1; j <= n_cols; ++j)
		{
			const Size i = match_col[j];
			if (i == 0)
				continue;

			const Size u_node = left_nodes[i - 1];
			const Size v_node = right_nodes[j - 1];
			const Size edge_id = edge_matrix[i - 1][j - 1];

			push_flow(source_edge[u_node], 1);
			push_flow(edge_id, 1);
			push_flow(sink_edge[v_node], 1);
		}

		Long total_cost = 0;
		for (Size id = 0; id < edges.size(); id += 2)
			total_cost += edges[id].flow * edges[id].cost;

		return total_cost;
	}

private:
	std::vector<Size> source_edge;
	std::vector<Size> sink_edge;
	std::vector<Size> left_index;
	std::vector<Size> right_index;

	bool extract_assignment_matrix(
	    const Size source, const Size sink, std::vector<Size> &left_nodes,
	    std::vector<Size> &right_nodes, std::vector<std::vector<Long>> &cost_matrix,
	    std::vector<std::vector<Size>> &edge_matrix
	)
	{
		std::fill(source_edge.begin(), source_edge.end(), MAX);
		std::fill(sink_edge.begin(), sink_edge.end(), MAX);
		std::fill(left_index.begin(), left_index.end(), MAX);
		std::fill(right_index.begin(), right_index.end(), MAX);

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
			left_index[u] = left_nodes.size();
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
				right_index[u] = right_nodes.size();
				right_nodes.push_back(u);
			}
		}

		if (left_nodes.size() > right_nodes.size())
			return false;

		cost_matrix.assign(
		    left_nodes.size(), std::vector<Long>(right_nodes.size(), INF)
		);
		edge_matrix.assign(
		    left_nodes.size(), std::vector<Size>(right_nodes.size(), MAX)
		);

		for (Size id = 0; id < edges.size(); id += 2)
		{
			const Size u = edges[id].from;
			const Size v = edges[id].to;
			const Long cap = edges[id].capacity;

			if (cap <= 0 || u == source || v == sink)
				continue;

			if (side[u] != 1 || side[v] != 2)
				return false;

			const Size r = left_index[u];
			const Size c = right_index[v];
			const Long total_edge_cost = edges[source_edge[u]].cost +
			                             edges[id].cost + edges[sink_edge[v]].cost;

			if (total_edge_cost < cost_matrix[r][c])
			{
				cost_matrix[r][c] = total_edge_cost;
				edge_matrix[r][c] = id;
			}
		}

		return true;
	}

	Long compute_fallback_min_cost_flow(const Size source, const Size sink)
	{
		for (Size id = 0; id < edges.size(); ++id)
			edges[id].flow = 0;

		std::vector<Long> potential(size, 0);
		std::vector<Long> distance(size, INF);
		std::vector<Size> parent_edge(size, MAX);

		while (true)
		{
			std::fill(distance.begin(), distance.end(), INF);
			std::fill(parent_edge.begin(), parent_edge.end(), MAX);

			using Pair = std::pair<Long, Size>;
			std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

			distance[source] = 0;
			pq.push({0, source});

			while (!pq.empty())
			{
				const auto [d, curr] = pq.top();
				pq.pop();

				if (d > distance[curr])
					continue;

				for (const Size edge_id : adjacency[curr])
				{
					if (get_residual_capacity(edge_id) <= 0)
						continue;

					const Size next = edges[edge_id].to;
					const Long reduced_cost = edges[edge_id].cost + potential[curr] -
					                          potential[next];

					if (distance[curr] + reduced_cost < distance[next])
					{
						distance[next] = distance[curr] + reduced_cost;
						parent_edge[next] = edge_id;
						pq.push({distance[next], next});
					}
				}
			}

			if (distance[sink] == INF)
				break;

			for (Size i = 0; i < size; ++i)
			{
				if (distance[i] != INF)
					potential[i] += distance[i];
			}

			Long bottleneck = INF;
			for (Size curr = sink; curr != source;
			     curr = edges[parent_edge[curr]].from)
			{
				bottleneck = std::min(
				    bottleneck, get_residual_capacity(parent_edge[curr])
				);
			}

			for (Size curr = sink; curr != source;
			     curr = edges[parent_edge[curr]].from)
			{
				push_flow(parent_edge[curr], bottleneck);
			}
		}

		Long total_cost = 0;
		for (Size id = 0; id < edges.size(); id += 2)
			total_cost += edges[id].flow * edges[id].cost;

		return total_cost;
	}
};

#endif // KUHN_MUNKRES_HPP

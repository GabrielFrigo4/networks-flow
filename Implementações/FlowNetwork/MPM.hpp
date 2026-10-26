#ifndef MPM_HPP
#define MPM_HPP

#include "FlowNetwork.hpp"
#include <algorithm>
#include <queue>
#include <vector>

class MPM : public FlowNetwork
{
public:
	explicit MPM(const Size n)
	    : FlowNetwork(n), distance_level(n), capacity_in(n), capacity_out(n),
	      excess(n), ptr_in(n), ptr_out(n), edges_in(n), edges_out(n), alive(n),
	      in_queue(n)
	{
	}

	static std::unique_ptr<FlowNetwork> create(const Size n)
	{
		return std::make_unique<MPM>(n);
	}

	std::unique_ptr<FlowNetwork> make(const Size n) const override
	{
		return std::make_unique<MPM>(n);
	}

	std::unique_ptr<FlowNetwork> clone() const override
	{
		return std::make_unique<MPM>(*this);
	}

	Long compute_max_flow(const Size source, const Size sink) override
	{
		Long total_flow = 0;

		while (build_level_graph(source, sink))
		{
			initialize_phase(source, sink);

			while (true)
			{
				prune_zero_nodes(source, sink);
				if (!alive[source] || !alive[sink])
					break;

				Size ref_node = MAX;
				Long min_potential = INF;

				for (Size u = 0; u < size; ++u)
				{
					if (!alive[u])
						continue;

					const Long pot = node_potential(u, source, sink);
					if (pot < min_potential)
					{
						min_potential = pot;
						ref_node = u;
					}
				}

				if (ref_node == MAX || min_potential <= 0)
					break;

				push_from_reference(ref_node, sink, min_potential);
				pull_to_reference(ref_node, source, min_potential);
				total_flow += min_potential;

				remove_node(ref_node);
			}
		}

		return total_flow;
	}

private:
	std::vector<Size> distance_level;
	std::vector<Long> capacity_in;
	std::vector<Long> capacity_out;
	std::vector<Long> excess;
	std::vector<Size> ptr_in;
	std::vector<Size> ptr_out;
	std::vector<std::vector<Size>> edges_in;
	std::vector<std::vector<Size>> edges_out;
	std::vector<bool> alive;
	std::vector<bool> in_queue;

	Long node_potential(const Size u, const Size source, const Size sink) const
	{
		if (u == source)
			return capacity_out[u];
		if (u == sink)
			return capacity_in[u];
		return std::min(capacity_in[u], capacity_out[u]);
	}

	bool build_level_graph(const Size source, const Size sink)
	{
		std::fill(distance_level.begin(), distance_level.end(), MAX);
		std::queue<Size> q;

		distance_level[source] = 0;
		q.push(source);

		while (!q.empty())
		{
			const Size u = q.front();
			q.pop();

			if (u == sink)
				continue;

			for (const Size edge_id : adjacency[u])
			{
				const Size v = edges[edge_id].to;
				if (get_residual_capacity(edge_id) > 0 && distance_level[v] == MAX)
				{
					distance_level[v] = distance_level[u] + 1;
					q.push(v);
				}
			}
		}

		return distance_level[sink] != MAX;
	}

	void initialize_phase(const Size source, const Size sink)
	{
		std::fill(capacity_in.begin(), capacity_in.end(), 0);
		std::fill(capacity_out.begin(), capacity_out.end(), 0);
		std::fill(ptr_in.begin(), ptr_in.end(), 0);
		std::fill(ptr_out.begin(), ptr_out.end(), 0);
		std::fill(alive.begin(), alive.end(), false);

		for (Size u = 0; u < size; ++u)
		{
			edges_in[u].clear();
			edges_out[u].clear();
			if (distance_level[u] < distance_level[sink] || u == sink)
				alive[u] = (distance_level[u] != MAX);
		}

		for (Size u = 0; u < size; ++u)
		{
			if (!alive[u] || u == sink)
				continue;

			for (const Size edge_id : adjacency[u])
			{
				const Size v = edges[edge_id].to;
				const Long cap = get_residual_capacity(edge_id);

				if (cap > 0 && alive[v] &&
				    distance_level[v] == distance_level[u] + 1)
				{
					edges_out[u].push_back(edge_id);
					edges_in[v].push_back(edge_id);
					capacity_out[u] += cap;
					capacity_in[v] += cap;
				}
			}
		}

		(void)source;
	}

	void remove_node(const Size u)
	{
		if (!alive[u])
			return;

		alive[u] = false;

		for (Size i = ptr_out[u]; i < edges_out[u].size(); ++i)
		{
			const Size edge_id = edges_out[u][i];
			const Size v = edges[edge_id].to;
			if (alive[v])
				capacity_in[v] -= get_residual_capacity(edge_id);
		}

		for (Size i = ptr_in[u]; i < edges_in[u].size(); ++i)
		{
			const Size edge_id = edges_in[u][i];
			const Size v = edges[edge_id].from;
			if (alive[v])
				capacity_out[v] -= get_residual_capacity(edge_id);
		}
	}

	void prune_zero_nodes(const Size source, const Size sink)
	{
		std::queue<Size> q;
		for (Size u = 0; u < size; ++u)
		{
			if (alive[u] && node_potential(u, source, sink) == 0)
				q.push(u);
		}

		while (!q.empty())
		{
			const Size u = q.front();
			q.pop();

			if (!alive[u])
				continue;

			alive[u] = false;

			for (Size i = ptr_out[u]; i < edges_out[u].size(); ++i)
			{
				const Size edge_id = edges_out[u][i];
				const Size v = edges[edge_id].to;
				if (alive[v])
				{
					capacity_in[v] -= get_residual_capacity(edge_id);
					if (node_potential(v, source, sink) == 0)
						q.push(v);
				}
			}

			for (Size i = ptr_in[u]; i < edges_in[u].size(); ++i)
			{
				const Size edge_id = edges_in[u][i];
				const Size v = edges[edge_id].from;
				if (alive[v])
				{
					capacity_out[v] -= get_residual_capacity(edge_id);
					if (node_potential(v, source, sink) == 0)
						q.push(v);
				}
			}
		}
	}

	void push_from_reference(const Size ref_node, const Size sink, const Long delta)
	{
		std::fill(excess.begin(), excess.end(), 0);
		std::fill(in_queue.begin(), in_queue.end(), false);

		std::queue<Size> q;
		excess[ref_node] = delta;
		q.push(ref_node);
		in_queue[ref_node] = true;

		while (!q.empty())
		{
			const Size u = q.front();
			q.pop();

			if (u == sink)
				continue;

			while (excess[u] > 0 && ptr_out[u] < edges_out[u].size())
			{
				const Size edge_id = edges_out[u][ptr_out[u]];
				const Size v = edges[edge_id].to;

				if (!alive[v])
				{
					++ptr_out[u];
					continue;
				}

				const Long cap = get_residual_capacity(edge_id);
				if (cap <= 0)
				{
					++ptr_out[u];
					continue;
				}

				const Long pushed = std::min(excess[u], cap);
				push_flow(edge_id, pushed);
				excess[u] -= pushed;
				excess[v] += pushed;
				capacity_out[u] -= pushed;
				capacity_in[v] -= pushed;

				if (v != sink && !in_queue[v])
				{
					in_queue[v] = true;
					q.push(v);
				}

				if (get_residual_capacity(edge_id) == 0)
					++ptr_out[u];
			}
		}
	}

	void pull_to_reference(const Size ref_node, const Size source, const Long delta)
	{
		std::fill(excess.begin(), excess.end(), 0);
		std::fill(in_queue.begin(), in_queue.end(), false);

		std::queue<Size> q;
		excess[ref_node] = delta;
		q.push(ref_node);
		in_queue[ref_node] = true;

		while (!q.empty())
		{
			const Size u = q.front();
			q.pop();

			if (u == source)
				continue;

			while (excess[u] > 0 && ptr_in[u] < edges_in[u].size())
			{
				const Size edge_id = edges_in[u][ptr_in[u]];
				const Size v = edges[edge_id].from;

				if (!alive[v])
				{
					++ptr_in[u];
					continue;
				}

				const Long cap = get_residual_capacity(edge_id);
				if (cap <= 0)
				{
					++ptr_in[u];
					continue;
				}

				const Long pulled = std::min(excess[u], cap);
				push_flow(edge_id, pulled);
				excess[u] -= pulled;
				excess[v] += pulled;
				capacity_in[u] -= pulled;
				capacity_out[v] -= pulled;

				if (v != source && !in_queue[v])
				{
					in_queue[v] = true;
					q.push(v);
				}

				if (get_residual_capacity(edge_id) == 0)
					++ptr_in[u];
			}
		}
	}
};

#endif // MPM_HPP

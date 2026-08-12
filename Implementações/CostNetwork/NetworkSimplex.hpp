#ifndef NETWORK_SIMPLEX_HPP
#define NETWORK_SIMPLEX_HPP

#include "CostNetwork.hpp"
#include <algorithm>
#include <numeric>

class NetworkSimplex : public CostNetwork
{
public:
	explicit NetworkSimplex(const Size n) : CostNetwork(n) {}

	static std::unique_ptr<CostNetwork> create(const Size n)
	{
		return std::make_unique<NetworkSimplex>(n);
	}

	std::unique_ptr<CostNetwork> make(const Size n) const override
	{
		return std::make_unique<NetworkSimplex>(n);
	}

	std::unique_ptr<CostNetwork> clone() const override
	{
		return std::make_unique<NetworkSimplex>(*this);
	}

	Long compute_min_cost_max_flow(const Size source, const Size sink) override
	{
		add_edge(sink, source, INF, 0);
		const Size back_edge = edges.size() - 2;

		const Size total_nodes = size + 1;
		const Size root = size;

		parent.assign(total_nodes, root);
		depth.assign(total_nodes, 1);
		thread.resize(total_nodes);
		potential.assign(total_nodes, 0);
		parent_edge.assign(total_nodes, MAX);
		arc_state.assign(edges.size() / 2, ArcState::LOWER);

		std::iota(thread.begin(), thread.end(), Size(1));
		thread[root] = 0;
		depth[root] = 0;

		const Long big_m = compute_big_m();

		for (Size v = 0; v < size; ++v)
		{
			const Size art_forward = edges.size();
			adjacency[v].push_back(edges.size());
			edges.push_back({v, root, big_m, big_m, 0});
			adjacency[root].push_back(edges.size());
			edges.push_back({root, v, 0, -big_m, 0});

			const Size art_reverse = edges.size();
			adjacency[root].push_back(edges.size());
			edges.push_back({root, v, big_m, big_m, 0});
			adjacency[v].push_back(edges.size());
			edges.push_back({v, root, 0, -big_m, 0});

			parent_edge[v] = art_forward;
			potential[v] = -big_m;
			arc_state.push_back(ArcState::TREE);
			arc_state.push_back(ArcState::TREE);
		}

		while (true)
		{
			const Size entering = find_entering_arc();
			if (entering == MAX)
				break;

			const Size leaving = find_leaving_arc(entering);
			pivot(entering, leaving);
		}

		Long total_cost = 0;
		for (Size i = 0; i < back_edge; i += 2)
			total_cost += edges[i].flow * edges[i].cost;
		return total_cost;
	}

private:
	enum class ArcState : uint8_t
	{
		LOWER,
		UPPER,
		TREE
	};

	std::vector<Size> parent;
	std::vector<Size> depth;
	std::vector<Size> thread;
	std::vector<Long> potential;
	std::vector<Size> parent_edge;
	std::vector<ArcState> arc_state;

	Long compute_big_m() const
	{
		Long sum = 1;
		for (const auto &edge : edges)
			if (edge.cost > 0)
				sum += edge.cost * edge.capacity;
		return sum;
	}

	[[nodiscard]] Long reduced_cost(const Size edge_id) const
	{
		return edges[edge_id].cost - potential[edges[edge_id].from] + potential[edges[edge_id].to];
	}

	Size find_entering_arc() const
	{
		Size best = MAX;
		Long best_violation = 0;

		for (Size i = 0; i < edges.size(); i += 2)
		{
			const Size arc_idx = i / 2;
			if (arc_state[arc_idx] == ArcState::TREE)
				continue;

			if (arc_state[arc_idx] == ArcState::LOWER)
			{
				const Long rc = reduced_cost(i);
				if (rc < best_violation)
				{
					best_violation = rc;
					best = i;
				}
			}
			else
			{
				const Long rc = reduced_cost(i);
				if (-rc < best_violation)
				{
					best_violation = -rc;
					best = i;
				}
			}
		}
		return best;
	}

	Size find_lca(Size u, Size v) const
	{
		while (u != v)
		{
			if (depth[u] < depth[v])
				std::swap(u, v);
			u = parent[u];
		}
		return u;
	}

	Size find_leaving_arc(const Size entering)
	{
		const Size arc_idx = entering / 2;
		Size u = edges[entering].from;
		Size v = edges[entering].to;

		if (arc_state[arc_idx] == ArcState::UPPER)
			std::swap(u, v);

		const Size lca = find_lca(u, v);

		Long bottleneck = get_residual_capacity(entering);
		Size leaving = entering;

		Size node = u;
		while (node != lca)
		{
			const Size eid = parent_edge[node];
			Long residual;
			if (edges[eid].from == node)
				residual = edges[eid ^ 1ULL].flow;
			else
				residual = get_residual_capacity(eid);

			if (residual < bottleneck)
			{
				bottleneck = residual;
				leaving = eid;
			}
			node = parent[node];
		}

		node = v;
		while (node != lca)
		{
			const Size eid = parent_edge[node];
			Long residual;
			if (edges[eid].to == node)
				residual = edges[eid ^ 1ULL].flow;
			else
				residual = get_residual_capacity(eid);

			if (residual <= bottleneck)
			{
				bottleneck = residual;
				leaving = eid;
			}
			node = parent[node];
		}

		Size aug_node = u;
		while (aug_node != lca)
		{
			const Size eid = parent_edge[aug_node];
			if (edges[eid].from == aug_node)
				push_flow(eid ^ 1ULL, bottleneck);
			else
				push_flow(eid, bottleneck);
			aug_node = parent[aug_node];
		}

		aug_node = v;
		while (aug_node != lca)
		{
			const Size eid = parent_edge[aug_node];
			if (edges[eid].to == aug_node)
				push_flow(eid ^ 1ULL, bottleneck);
			else
				push_flow(eid, bottleneck);
			aug_node = parent[aug_node];
		}

		if (arc_state[arc_idx] == ArcState::LOWER)
			push_flow(entering, bottleneck);
		else
			push_flow(entering ^ 1ULL, bottleneck);

		return leaving;
	}

	void pivot(const Size entering, const Size leaving)
	{
		const Size entering_arc = entering / 2;
		const Size leaving_arc = leaving / 2;

		if (entering == leaving)
		{
			if (arc_state[entering_arc] == ArcState::LOWER)
				arc_state[entering_arc] = ArcState::UPPER;
			else
				arc_state[entering_arc] = ArcState::LOWER;
			return;
		}

		if (get_residual_capacity(leaving) == 0)
			arc_state[leaving_arc] = ArcState::UPPER;
		else
			arc_state[leaving_arc] = ArcState::LOWER;

		arc_state[entering_arc] = ArcState::TREE;

		update_tree(entering, leaving);
	}

	void update_tree(const Size entering, const Size leaving)
	{
		Size u = edges[entering].from;
		Size v = edges[entering].to;

		Size leave_u = edges[leaving].from;
		Size leave_v = edges[leaving].to;
		if (depth[leave_u] < depth[leave_v])
			std::swap(leave_u, leave_v);

		std::vector<Size> path_u, path_v;
		Size node = u;
		while (node != leave_v && node != leave_u)
		{
			path_u.push_back(node);
			node = parent[node];
		}
		bool u_side = (node == leave_u);

		if (!u_side)
		{
			std::swap(u, v);
			path_u.clear();
			node = u;
			while (node != leave_u)
			{
				path_u.push_back(node);
				node = parent[node];
			}
		}

		std::vector<Size> reroot_path;
		node = leave_u;
		while (true)
		{
			reroot_path.push_back(node);
			if (node == u)
				break;
			node = parent[node];
		}

		for (Size i = reroot_path.size() - 1; i > 0; --i)
		{
			const Size child = reroot_path[i];
			const Size new_parent = reroot_path[i - 1];
			parent[child] = new_parent;
			parent_edge[child] = parent_edge[new_parent];
		}

		parent[leave_u] = leave_v;
		parent_edge[leave_u] = entering;

		rebuild_depth_and_thread();
	}

	void rebuild_depth_and_thread()
	{
		const Size root = size;
		depth[root] = 0;

		std::vector<std::vector<Size>> children(size + 1);
		for (Size v = 0; v <= size; ++v)
			if (v != root)
				children[parent[v]].push_back(v);

		Size prev = root;
		std::vector<Size> stack = {root};
		while (!stack.empty())
		{
			const Size node = stack.back();
			stack.pop_back();

			if (node != root)
				depth[node] = depth[parent[node]] + 1;

			thread[prev] = node;
			prev = node;

			for (auto it = children[node].rbegin(); it != children[node].rend(); ++it)
				stack.push_back(*it);
		}
		thread[prev] = root;

		for (Size v = 0; v <= size; ++v)
		{
			if (v == root || parent_edge[v] == MAX)
				continue;
			const Long rc = reduced_cost(parent_edge[v]);
			if (rc != 0)
			{
				potential[v] += rc;
				propagate_potential(v, children);
			}
		}
	}

	void propagate_potential(const Size node, const std::vector<std::vector<Size>> &children)
	{
		for (const Size child : children[node])
		{
			const Long rc = reduced_cost(parent_edge[child]);
			if (rc != 0)
			{
				potential[child] += rc;
				propagate_potential(child, children);
			}
		}
	}
};

#endif // NETWORK_SIMPLEX_HPP

#ifndef NETWORK_SIMPLEX_HPP
#define NETWORK_SIMPLEX_HPP

#include "CostNetwork.hpp"
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <queue>
#include <vector>

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
		Long max_c = 1;
		for (const auto &e : edges)
			max_c = std::max(max_c, std::abs(e.cost));

		const Long back_cost = -(max_c * static_cast<Long>(size + 2) + 1);
		const Size back_edge_idx = edges.size();
		add_edge(sink, source, INF, back_cost);

		const Size num_nodes = size + 1;
		const Size root = size;
		adjacency.resize(num_nodes);

		parent.assign(num_nodes, root);
		depth.assign(num_nodes, 1);
		potential.assign(num_nodes, 0);
		parent_edge.assign(num_nodes, MAX);
		tree_adj.assign(num_nodes, {});

		arc_state.assign(edges.size() / 2, ArcState::LOWER);

		const Long big_m = std::abs(back_cost) * static_cast<Long>(size + 2) + 100000;

		depth[root] = 0;
		potential[root] = 0;

		for (Size v = 0; v < size; ++v)
		{
			const Size art_id = edges.size();
			adjacency[v].push_back(art_id);
			edges.push_back({v, root, INF, big_m, 0});
			adjacency[root].push_back(art_id + 1);
			edges.push_back({root, v, 0, -big_m, 0});

			parent[v] = root;
			parent_edge[v] = art_id;
			potential[v] = -big_m;
			arc_state.push_back(ArcState::TREE);
			tree_adj[root].push_back(v);
			tree_adj[v].push_back(root);
		}

		while (true)
		{
			Size entering = MAX;
			Long best_violation = 0;

			for (Size i = 0; i < edges.size(); i += 2)
			{
				const Size arc_idx = i / 2;
				if (arc_state[arc_idx] == ArcState::TREE)
					continue;

				const Long rc = reduced_cost(i);
				if (arc_state[arc_idx] == ArcState::LOWER && rc < best_violation)
				{
					best_violation = rc;
					entering = i;
				}
				else if (arc_state[arc_idx] == ArcState::UPPER && -rc < best_violation)
				{
					best_violation = -rc;
					entering = i;
				}
			}

			if (entering == MAX)
				break;

			pivot(entering);
		}

		Long total_cost = 0;
		for (Size i = 0; i < back_edge_idx; i += 2)
		{
			total_cost += edges[i].flow * edges[i].cost;
		}

		adjacency[sink].pop_back();
		adjacency[source].pop_back();
		for (Size v = 0; v < size; ++v)
		{
			adjacency[v].pop_back();
		}
		adjacency.resize(size);
		edges.resize(back_edge_idx);

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
	std::vector<Long> potential;
	std::vector<Size> parent_edge;
	std::vector<ArcState> arc_state;
	std::vector<std::vector<Size>> tree_adj;

	[[nodiscard]] Long reduced_cost(const Size edge_id) const
	{
		return edges[edge_id].cost + potential[edges[edge_id].from] - potential[edges[edge_id].to];
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

	void pivot(const Size entering)
	{
		const Size arc_idx = entering / 2;
		Size u = edges[entering].from;
		Size v = edges[entering].to;

		const bool is_upper = (arc_state[arc_idx] == ArcState::UPPER);
		if (is_upper)
			std::swap(u, v);

		const Size lca = find_lca(u, v);

		Long bottleneck = is_upper ? edges[entering].flow : (edges[entering].capacity - edges[entering].flow);
		Size leaving_edge = entering;
		bool leaving_is_entering = true;

		for (Size curr = v; curr != lca; curr = parent[curr])
		{
			const Size pe = parent_edge[curr];
			Long cap;
			if (edges[pe].from == curr)
				cap = edges[pe].capacity - edges[pe].flow;
			else
				cap = edges[pe].flow;

			if (cap < bottleneck)
			{
				bottleneck = cap;
				leaving_edge = pe;
				leaving_is_entering = false;
			}
		}

		for (Size curr = u; curr != lca; curr = parent[curr])
		{
			const Size pe = parent_edge[curr];
			Long cap;
			if (edges[pe].to == curr)
				cap = edges[pe].capacity - edges[pe].flow;
			else
				cap = edges[pe].flow;

			if (cap <= bottleneck)
			{
				bottleneck = cap;
				leaving_edge = pe;
				leaving_is_entering = false;
			}
		}

		if (bottleneck == 0 && leaving_is_entering)
		{
			arc_state[arc_idx] = is_upper ? ArcState::LOWER : ArcState::UPPER;
			return;
		}

		if (!is_upper)
		{
			edges[entering].flow += bottleneck;
			edges[entering ^ 1ULL].flow -= bottleneck;
		}
		else
		{
			edges[entering].flow -= bottleneck;
			edges[entering ^ 1ULL].flow += bottleneck;
		}

		for (Size curr = v; curr != lca; curr = parent[curr])
		{
			const Size pe = parent_edge[curr];
			if (edges[pe].from == curr)
			{
				edges[pe].flow += bottleneck;
				edges[pe ^ 1ULL].flow -= bottleneck;
			}
			else
			{
				edges[pe].flow -= bottleneck;
				edges[pe ^ 1ULL].flow += bottleneck;
			}
		}

		for (Size curr = u; curr != lca; curr = parent[curr])
		{
			const Size pe = parent_edge[curr];
			if (edges[pe].to == curr)
			{
				edges[pe].flow += bottleneck;
				edges[pe ^ 1ULL].flow -= bottleneck;
			}
			else
			{
				edges[pe].flow -= bottleneck;
				edges[pe ^ 1ULL].flow += bottleneck;
			}
		}

		if (leaving_is_entering)
		{
			arc_state[arc_idx] = (edges[entering].flow == edges[entering].capacity) ? ArcState::UPPER : ArcState::LOWER;
			return;
		}

		const Size leaving_arc_idx = leaving_edge / 2;
		arc_state[leaving_arc_idx] = (edges[leaving_edge].flow == 0) ? ArcState::LOWER : ArcState::UPPER;
		arc_state[arc_idx] = ArcState::TREE;

		const Size lu = edges[leaving_edge].from;
		const Size lv = edges[leaving_edge].to;
		auto &adj_lu = tree_adj[lu];
		adj_lu.erase(std::remove(adj_lu.begin(), adj_lu.end(), lv), adj_lu.end());
        auto &adj_lv = tree_adj[lv];
		adj_lv.erase(std::remove(adj_lv.begin(), adj_lv.end(), lu), adj_lv.end());

		const Size orig_u = edges[entering].from;
		const Size orig_v = edges[entering].to;
		tree_adj[orig_u].push_back(orig_v);
		tree_adj[orig_v].push_back(orig_u);

		const Size root = size;
		std::vector<bool> visited(size + 1, false);
		std::queue<Size> q;
		q.push(root);
		visited[root] = true;
		parent[root] = root;
		depth[root] = 0;
		parent_edge[root] = MAX;
		potential[root] = 0;

		while (!q.empty())
		{
			const Size curr = q.front();
			q.pop();

			for (const Size neighbor : tree_adj[curr])
			{
				if (visited[neighbor])
					continue;

				visited[neighbor] = true;
				parent[neighbor] = curr;
				depth[neighbor] = depth[curr] + 1;

				Size eid = MAX;
				for (const Size edge_id : adjacency[curr])
				{
					if (edges[edge_id].to == neighbor && arc_state[edge_id / 2] == ArcState::TREE)
					{
						eid = edge_id;
						break;
					}
				}
				parent_edge[neighbor] = eid;

				if (edges[eid].from == curr)
					potential[neighbor] = potential[curr] + edges[eid].cost;
				else
					potential[neighbor] = potential[curr] - edges[eid].cost;

				q.push(neighbor);
			}
		}
	}
};

#endif // NETWORK_SIMPLEX_HPP

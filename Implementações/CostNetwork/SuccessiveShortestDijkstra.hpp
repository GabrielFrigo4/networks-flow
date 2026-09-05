#ifndef SUCCESSIVE_SHORTEST_DIJKSTRA_HPP
#define SUCCESSIVE_SHORTEST_DIJKSTRA_HPP

#include "CostNetwork.hpp"
#include <algorithm>
#include <queue>
#include <vector>

class SuccessiveShortestDijkstra : public CostNetwork
{
public:
	explicit SuccessiveShortestDijkstra(const Size n)
	    : CostNetwork(n), potential(n, 0), distance(n), parent_edge(n)
	{
	}

	static std::unique_ptr<CostNetwork> create(const Size n)
	{
		return std::make_unique<SuccessiveShortestDijkstra>(n);
	}

	std::unique_ptr<CostNetwork> make(const Size n) const override
	{
		return std::make_unique<SuccessiveShortestDijkstra>(n);
	}

	std::unique_ptr<CostNetwork> clone() const override
	{
		return std::make_unique<SuccessiveShortestDijkstra>(*this);
	}

	Long compute_min_cost_max_flow(const Size source, const Size sink) override
	{
		bool has_negative_cost = false;
		for (const auto &e : edges)
		{
			if (e.capacity > 0 && e.cost < 0)
			{
				has_negative_cost = true;
				break;
			}
		}

		std::fill(potential.begin(), potential.end(), 0);
		if (has_negative_cost)
		{
			std::fill(distance.begin(), distance.end(), INF);
			std::vector<bool> in_queue(size, false);
			std::queue<Size> q;

			distance[source] = 0;
			in_queue[source] = true;
			q.push(source);

			while (!q.empty())
			{
				const Size curr = q.front();
				q.pop();
				in_queue[curr] = false;

				for (const Size edge_id : adjacency[curr])
				{
					if (get_residual_capacity(edge_id) <= 0)
						continue;

					const Size next = edges[edge_id].to;
					const Long new_d = distance[curr] + edges[edge_id].cost;

					if (new_d < distance[next])
					{
						distance[next] = new_d;
						if (!in_queue[next])
						{
							in_queue[next] = true;
							q.push(next);
						}
					}
				}
			}

			for (Size i = 0; i < size; ++i)
			{
				if (distance[i] != INF)
					potential[i] = distance[i];
			}
		}

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
		for (Size i = 0; i < edges.size(); i += 2)
		{
			total_cost += edges[i].flow * edges[i].cost;
		}

		return total_cost;
	}

private:
	std::vector<Long> potential;
	std::vector<Long> distance;
	std::vector<Size> parent_edge;
};

#endif // SUCCESSIVE_SHORTEST_DIJKSTRA_HPP

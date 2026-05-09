# 💻 Implementações e Códigos

Abaixo estão listados os algoritmos já implementados no escopo deste projeto:

- 🐢 **[Edmonds-Karp][edmonds_karp]**
- 🚀 **[Dinic's Algorithm][dinic_algorithm]**
- ⚙️ **[Push-Relabel][push_relabel]**
- ⚡ **[Push-Relabel Improved][push_relabel_faster]**

## 🔗 Lista de Problemas

1.  [Fast Maximum Flow](https://www.spoj.com/problems/FASTFLOW/)
2.  [Petya and Graph](https://codeforces.com/problemset/problem/1082/G)
3.  [Array and Operations](https://codeforces.com/problemset/problem/498/C)
4.  [Maximum Flow](https://codeforces.com/problemset/problem/843/E)
5.  [Array and Operations](https://codeforces.com/contest/498/problem/c)
6.  [Red-Blue Graph](https://codeforces.com/contest/1288/problem/f)

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
```

[edmonds_karp]: ./1.%20Edmonds-Karp/README.md
[dinic_algorithm]: ./2.%20Dinic's%20Algorithm/README.md
[push_relabel]: ./3.%20Push-Relabel/README.md
[push_relabel_faster]: ./4.%20Push-Relabel%20Improved/README.md

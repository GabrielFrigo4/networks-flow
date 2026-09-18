#ifndef DIMACS_MIN_HPP
#define DIMACS_MIN_HPP

#include <istream>
#include <limits>
#include <string>
#include <vector>

using Long = long long;
using Size = std::size_t;

inline constexpr Size DIMACS_MIN_SENTINEL = std::numeric_limits<Size>::max() >> 8;

struct DimacsMinCost
{
	Size num_nodes;
	Size source;
	Size sink;

	struct Arc
	{
		Size from, to;
		Long lower, capacity, cost;
	};
	std::vector<Arc> arcs;
};

inline DimacsMinCost parse_dimacs_min(std::istream &input)
{
	DimacsMinCost result{};
	result.source = DIMACS_MIN_SENTINEL;
	result.sink = DIMACS_MIN_SENTINEL;

	std::vector<std::pair<Size, Long>> node_supplies;

	char type{};
	while (input >> type)
	{
		if (type == 'c')
		{
			std::string comment;
			std::getline(input, comment);
		}
		else if (type == 'p')
		{
			std::string format;
			Size num_arcs{};
			input >> format >> result.num_nodes >> num_arcs;
			result.arcs.reserve(num_arcs);
		}
		else if (type == 'n')
		{
			Size node_id{};
			Long supply{};
			input >> node_id >> supply;
			node_supplies.emplace_back(node_id - 1, supply);
		}
		else if (type == 'a')
		{
			Size from{}, to{};
			Long lower{}, capacity{}, cost{};
			input >> from >> to >> lower >> capacity >> cost;
			result.arcs.push_back({from - 1, to - 1, lower, capacity, cost});
		}
	}

	Size pos_count = 0;
	Size neg_count = 0;
	Size single_pos = DIMACS_MIN_SENTINEL;
	Size single_neg = DIMACS_MIN_SENTINEL;

	for (const auto &[node, supply] : node_supplies)
	{
		if (supply > 0)
		{
			++pos_count;
			single_pos = node;
		}
		else if (supply < 0)
		{
			++neg_count;
			single_neg = node;
		}
	}

	if (pos_count == 1 && neg_count == 1)
	{
		result.source = single_pos;
		result.sink = single_neg;
	}
	else if (pos_count > 0 || neg_count > 0)
	{
		const Size super_source = result.num_nodes;
		const Size super_sink = result.num_nodes + 1;
		result.num_nodes += 2;
		result.source = super_source;
		result.sink = super_sink;

		for (const auto &[node, supply] : node_supplies)
		{
			if (supply > 0)
				result.arcs.push_back({super_source, node, 0, supply, 0});
			else if (supply < 0)
				result.arcs.push_back({node, super_sink, 0, -supply, 0});
		}
	}

	return result;
}

#endif // DIMACS_MIN_HPP

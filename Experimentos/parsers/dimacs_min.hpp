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
	result.num_nodes = 0;
	result.source = DIMACS_MIN_SENTINEL;
	result.sink = DIMACS_MIN_SENTINEL;

	bool is_asn = false;
	std::vector<std::pair<Size, Long>> node_supplies;

	std::string line;
	while (std::getline(input, line))
	{
		if (line.empty())
			continue;

		char type = line[0];

		if (type == 'c')
			continue;

		if (type == 'p')
		{
			std::string format;
			Size num_arcs{};
			std::string buf = line.substr(1);
			std::size_t pos = 0;

			while (pos < buf.size() && buf[pos] == ' ')
				++pos;

			std::size_t wend = pos;
			while (wend < buf.size() && buf[wend] != ' ')
				++wend;
			format = buf.substr(pos, wend - pos);
			pos = wend;
			is_asn = (format == "asn");

			while (pos < buf.size() && buf[pos] == ' ')
				++pos;
			result.num_nodes = std::stoull(buf.substr(pos));
			while (pos < buf.size() && buf[pos] != ' ')
				++pos;

			while (pos < buf.size() && buf[pos] == ' ')
				++pos;
			num_arcs = std::stoull(buf.substr(pos));
			result.arcs.reserve(num_arcs);
		}
		else if (type == 'n')
		{
			std::string buf = line.substr(1);
			std::vector<std::string> tokens;
			std::size_t pos = 0;
			while (pos < buf.size())
			{
				while (pos < buf.size() && buf[pos] == ' ')
					++pos;
				if (pos >= buf.size())
					break;
				std::size_t end = pos;
				while (end < buf.size() && buf[end] != ' ')
					++end;
				tokens.push_back(buf.substr(pos, end - pos));
				pos = end;
			}

			if (is_asn)
			{
				if (!tokens.empty())
				{
					const Size node_id = std::stoull(tokens[0]) - 1;
					node_supplies.emplace_back(node_id, 1LL);
				}
			}
			else
			{
				if (tokens.size() >= 2)
				{
					const Size node_id = std::stoull(tokens[0]) - 1;
					const Long supply = std::stoll(tokens[1]);
					node_supplies.emplace_back(node_id, supply);
				}
			}
		}
		else if (type == 'a')
		{
			std::string buf = line.substr(1);
			std::vector<std::string> tokens;
			std::size_t pos = 0;
			while (pos < buf.size())
			{
				while (pos < buf.size() && buf[pos] == ' ')
					++pos;
				if (pos >= buf.size())
					break;
				std::size_t end = pos;
				while (end < buf.size() && buf[end] != ' ')
					++end;
				tokens.push_back(buf.substr(pos, end - pos));
				pos = end;
			}

			if (is_asn)
			{
				if (tokens.size() >= 3)
				{
					const Size from = std::stoull(tokens[0]) - 1;
					const Size to = std::stoull(tokens[1]) - 1;
					const Long cost = std::stoll(tokens[2]);
					result.arcs.push_back({from, to, 0LL, 1LL, cost});
				}
			}
			else
			{
				if (tokens.size() >= 5)
				{
					const Size from = std::stoull(tokens[0]) - 1;
					const Size to = std::stoull(tokens[1]) - 1;
					const Long lower = std::stoll(tokens[2]);
					const Long capacity = std::stoll(tokens[3]);
					const Long cost = std::stoll(tokens[4]);
					result.arcs.push_back({from, to, lower, capacity, cost});
				}
			}
		}
	}

	if (is_asn)
	{
		const Size super_source = result.num_nodes;
		const Size super_sink = result.num_nodes + 1;
		result.num_nodes += 2;
		result.source = super_source;
		result.sink = super_sink;

		for (const auto &[node, supply] : node_supplies)
			result.arcs.push_back({super_source, node, 0LL, supply, 0LL});

		std::vector<bool> is_left(result.num_nodes, false);
		for (const auto &[node, supply] : node_supplies)
			is_left[node] = true;

		std::vector<bool> seen_right(result.num_nodes, false);
		for (const auto &arc : result.arcs)
		{
			if (arc.to < result.num_nodes - 2 && !is_left[arc.to] &&
			    !seen_right[arc.to])
			{
				seen_right[arc.to] = true;
				result.arcs.push_back({arc.to, super_sink, 0LL, 1LL, 0LL});
			}
		}
	}
	else
	{
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
					result.arcs.push_back({super_source, node, 0LL, supply, 0LL});
				else if (supply < 0)
					result.arcs.push_back({node, super_sink, 0LL, -supply, 0LL});
			}
		}
	}

	return result;
}

#endif // DIMACS_MIN_HPP

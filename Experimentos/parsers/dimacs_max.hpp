#ifndef DIMACS_MAX_HPP
#define DIMACS_MAX_HPP

#include <istream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using Long = long long;
using Size = std::size_t;

inline constexpr Size DIMACS_MAX_SENTINEL = std::numeric_limits<Size>::max() >> 8;

struct DimacsMaxFlow
{
	Size num_nodes;
	Size source;
	Size sink;

	struct Arc
	{
		Size from, to;
		Long capacity;
	};
	std::vector<Arc> arcs;
};

inline DimacsMaxFlow parse_dimacs_max(std::istream &input)
{
	DimacsMaxFlow result{};
	result.source = DIMACS_MAX_SENTINEL;
	result.sink = DIMACS_MAX_SENTINEL;

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
			char node_type{};
			input >> node_id >> node_type;
			if (node_type == 's')
			{
				if (result.source != DIMACS_MAX_SENTINEL)
					throw std::runtime_error("Múltiplas fontes declaradas.");
				result.source = node_id - 1;
			}
			else if (node_type == 't')
			{
				if (result.sink != DIMACS_MAX_SENTINEL)
					throw std::runtime_error("Múltiplos sorvedouros declarados.");
				result.sink = node_id - 1;
			}
		}
		else if (type == 'a')
		{
			Size from{}, to{};
			Long capacity{};
			input >> from >> to >> capacity;
			result.arcs.push_back({from - 1, to - 1, capacity});
		}
	}

	if (result.source == DIMACS_MAX_SENTINEL || result.sink == DIMACS_MAX_SENTINEL)
		throw std::runtime_error("Fonte ou sorvedouro ausente.");

	return result;
}

#endif // DIMACS_MAX_HPP

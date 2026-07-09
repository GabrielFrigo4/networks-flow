#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../../Implementações/FlowNetwork.hpp"

enum class SeedLabel
{
	NONE,
	FOREGROUND,
	BACKGROUND
};

struct Seed
{
	Size x, y;
	SeedLabel label;
};

inline std::vector<Seed> read_seeds(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "error: cannot open seeds file '" << filename << "'\n";
		std::exit(1);
	}

	std::vector<Seed> seeds;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		Size x, y;
		char label_char;

		if (!(iss >> x >> y >> label_char))
			continue;

		SeedLabel label = SeedLabel::NONE;
		if (label_char == 'F' || label_char == 'f')
			label = SeedLabel::FOREGROUND;
		else if (label_char == 'B' || label_char == 'b')
			label = SeedLabel::BACKGROUND;

		if (label != SeedLabel::NONE)
			seeds.push_back({x, y, label});
	}

	return seeds;
}

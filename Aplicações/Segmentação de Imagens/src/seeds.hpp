#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../../Implementações/FlowNetwork.hpp"
#include "image.hpp"

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

inline std::vector<Seed> read_seeds_from_ppm(const std::string &filename)
{
	const Image img = read_ppm(filename);
	std::vector<Seed> seeds;

	for (Size y = 0; y < img.height; y++)
	{
		for (Size x = 0; x < img.width; x++)
		{
			const Pixel &px = img.at(x, y);
			if (px.r == 255 && px.g == 0 && px.b == 0)
				seeds.push_back({x, y, SeedLabel::FOREGROUND});
			else if (px.r == 0 && px.g == 0 && px.b == 255)
				seeds.push_back({x, y, SeedLabel::BACKGROUND});
		}
	}
	return seeds;
}

inline std::vector<Seed> read_seeds(const std::string &filename)
{
	if (filename.size() >= 4)
	{
		std::string ext = filename.substr(filename.size() - 4);
		for (char &c : ext)
			c = static_cast<char>(std::tolower(c));
		if (ext == ".ppm")
			return read_seeds_from_ppm(filename);
	}

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

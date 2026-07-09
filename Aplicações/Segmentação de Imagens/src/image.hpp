#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../../Implementações/FlowNetwork.hpp"

struct Pixel
{
	unsigned char r, g, b;
};

struct Image
{
	Size width, height;
	std::vector<Pixel> data;

	[[nodiscard]] Size index(const Size x, const Size y) const
	{
		return y * width + x;
	}

	[[nodiscard]] const Pixel &at(const Size x, const Size y) const
	{
		return data[index(x, y)];
	}
};

inline Image read_ppm(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "error: cannot open '" << filename << "'\n";
		std::exit(1);
	}

	std::string magic;
	file >> magic;

	if (magic != "P3" && magic != "P6")
	{
		std::cerr << "error: unsupported PPM format '" << magic << "'\n";
		std::exit(1);
	}

	char c;
	file.get(c);
	while (file.peek() == '#')
	{
		std::string line;
		std::getline(file, line);
	}

	Image img;
	Size max_val;
	file >> img.width >> img.height >> max_val;
	img.data.resize(img.width * img.height);

	if (magic == "P3")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int r, g, b;
			file >> r >> g >> b;
			img.data[i] = {
			    static_cast<unsigned char>(r),
			    static_cast<unsigned char>(g),
			    static_cast<unsigned char>(b)
			};
		}
	}
	else
	{
		file.get(c);
		for (Size i = 0; i < img.width * img.height; i++)
			file.read(reinterpret_cast<char *>(&img.data[i]), 3);
	}

	return img;
}

inline void write_ppm(const std::string &filename, const Image &img)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "error: cannot create '" << filename << "'\n";
		std::exit(1);
	}

	file << "P6\n" << img.width << " " << img.height << "\n255\n";
	for (Size i = 0; i < img.width * img.height; i++)
		file.write(reinterpret_cast<const char *>(&img.data[i]), 3);
}

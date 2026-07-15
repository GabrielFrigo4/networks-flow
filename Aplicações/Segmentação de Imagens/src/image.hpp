#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../../Implementações/FlowNetwork.hpp"

struct Pixel
{
	unsigned char r, g, b;
};

struct Image
{
	Size                width, height;
	std::vector<Pixel>  data;
	bool                is_ascii = false;

	[[nodiscard]] Size index(const Size x, const Size y) const { return y * width + x; }

	[[nodiscard]] const Pixel &at(const Size x, const Size y) const { return data[index(x, y)]; }
	[[nodiscard]] const Pixel &at(const int  x, const int  y) const
	{
		return data[index(static_cast<Size>(x), static_cast<Size>(y))];
	}
};

inline void skip_comments(std::ifstream &file)
{
	char c;
	while (file >> std::ws && file.peek() == '#')
	{
		std::string line;
		std::getline(file, line);
	}
}

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

	if (magic.empty() || magic[0] != 'P' || magic.size() != 2 || magic[1] < '1' || magic[1] > '7')
	{
		std::cerr << "error: unsupported Netpbm format '" << magic << "'\n";
		std::exit(1);
	}

	Image  img;
	img.is_ascii = (magic == "P1" || magic == "P2" || magic == "P3");
	Size   max_val = 255;
	int    depth   = 3;

	if (magic == "P7")
	{
		std::string token;
		while (file >> token && token != "ENDHDR")
		{
			if      (token == "WIDTH")    file >> img.width;
			else if (token == "HEIGHT")   file >> img.height;
			else if (token == "DEPTH")    file >> depth;
			else if (token == "MAXVAL")   file >> max_val;
			else if (token == "TUPLTYPE") { std::string s; file >> s; }
			else if (token == "#")        { std::string s; std::getline(file, s); }
		}
		char c;
		file.get(c);
	}
	else
	{
		skip_comments(file);
		file >> img.width;
		skip_comments(file);
		file >> img.height;
		if (magic != "P1" && magic != "P4")
		{
			skip_comments(file);
			file >> max_val;
		}
		char c;
		file.get(c);
	}

	img.data.resize(img.width * img.height);

	auto scale = [max_val](int val) -> unsigned char {
		if (max_val == 255) return static_cast<unsigned char>(val);
		return static_cast<unsigned char>((val * 255) / max_val);
	};

	if (magic == "P1")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int v;
			file >> v;
			const unsigned char c = (v == 1) ? 0 : 255;
			img.data[i] = {c, c, c};
		}
	}
	else if (magic == "P2")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int v;
			file >> v;
			const unsigned char c = scale(v);
			img.data[i] = {c, c, c};
		}
	}
	else if (magic == "P3")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int r, g, b;
			file >> r >> g >> b;
			img.data[i] = {scale(r), scale(g), scale(b)};
		}
	}
	else if (magic == "P4")
	{
		const Size row_bytes = (img.width + 7) / 8;
		std::vector<unsigned char> row(row_bytes);
		Size idx = 0;
		for (Size y = 0; y < img.height; y++)
		{
			file.read(reinterpret_cast<char *>(row.data()), row_bytes);
			for (Size x = 0; x < img.width; x++)
			{
				const int  bit = (row[x / 8] >> (7 - (x % 8))) & 1;
				const unsigned char c = (bit == 1) ? 0 : 255;
				img.data[idx++] = {c, c, c};
			}
		}
	}
	else if (magic == "P5")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			unsigned char v;
			file.read(reinterpret_cast<char *>(&v), 1);
			const unsigned char c = scale(v);
			img.data[i] = {c, c, c};
		}
	}
	else if (magic == "P6")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			unsigned char rgb[3];
			file.read(reinterpret_cast<char *>(rgb), 3);
			img.data[i] = {scale(rgb[0]), scale(rgb[1]), scale(rgb[2])};
		}
	}
	else if (magic == "P7")
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			std::vector<unsigned char> channels(depth);
			file.read(reinterpret_cast<char *>(channels.data()), depth);
			if (depth >= 3)
				img.data[i] = {scale(channels[0]), scale(channels[1]), scale(channels[2])};
			else
			{
				const unsigned char c = scale(channels[0]);
				img.data[i] = {c, c, c};
			}
		}
	}

	return img;
}

inline void write_ppm(const std::string &filename, const Image &img, bool ascii = false)
{
	if (ascii)
	{
		std::ofstream file(filename);
		if (!file.is_open())
		{
			std::cerr << "error: cannot create '" << filename << "'\n";
			std::exit(1);
		}
		file << "P3\n" << img.width << " " << img.height << "\n255\n";
		for (Size i = 0; i < img.width * img.height; i++)
		{
			file << static_cast<int>(img.data[i].r) << " "
			     << static_cast<int>(img.data[i].g) << " "
			     << static_cast<int>(img.data[i].b) << " ";
			if ((i + 1) % 8 == 0)
				file << "\n";
		}
		if ((img.width * img.height) % 8 != 0)
			file << "\n";
	}
	else
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
}

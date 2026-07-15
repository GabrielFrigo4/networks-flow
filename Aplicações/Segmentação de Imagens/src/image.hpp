#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdint>

#include "../../../Implementações/FlowNetwork.hpp"

struct Pixel
{
	unsigned char r, g, b;
};

struct Image
{
	Size width, height;
	std::vector<Pixel> data;
	bool is_ascii = false;

	[[nodiscard]] Size index(const Size x, const Size y) const
	{
		return y * width + x;
	}

	[[nodiscard]] const Pixel &at(const Size x, const Size y) const
	{
		return data[index(x, y)];
	}
};

// Helper to skip comments and whitespace in ASCII formats
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

	Image img;
	img.is_ascii = (magic == "P1" || magic == "P2" || magic == "P3");
	Size max_val = 255;
	int depth = 3;

	if (magic == "P7")
	{
		std::string token;
		while (file >> token && token != "ENDHDR")
		{
			if (token == "WIDTH") file >> img.width;
			else if (token == "HEIGHT") file >> img.height;
			else if (token == "DEPTH") file >> depth;
			else if (token == "MAXVAL") file >> max_val;
			else if (token == "TUPLTYPE")
			{
				std::string tupltype;
				file >> tupltype;
			}
			else if (token == "#")
			{
				std::string line;
				std::getline(file, line);
			}
		}
		char c;
		file.get(c); // consume trailing newline after ENDHDR
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
		file.get(c); // consume trailing whitespace/newline
	}

	img.data.resize(img.width * img.height);

	auto scale = [max_val](int val) -> unsigned char {
		if (max_val == 255) return static_cast<unsigned char>(val);
		return static_cast<unsigned char>((val * 255) / max_val);
	};

	if (magic == "P1") // ASCII Bitmap
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int v;
			file >> v;
			unsigned char color = (v == 1) ? 0 : 255;
			img.data[i] = {color, color, color};
		}
	}
	else if (magic == "P2") // ASCII Graymap
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int v;
			file >> v;
			unsigned char color = scale(v);
			img.data[i] = {color, color, color};
		}
	}
	else if (magic == "P3") // ASCII Pixmap
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			int r, g, b;
			file >> r >> g >> b;
			img.data[i] = {scale(r), scale(g), scale(b)};
		}
	}
	else if (magic == "P4") // Binary Bitmap
	{
		Size row_bytes = (img.width + 7) / 8;
		std::vector<unsigned char> row(row_bytes);
		Size idx = 0;
		for (Size y = 0; y < img.height; y++)
		{
			file.read(reinterpret_cast<char *>(row.data()), row_bytes);
			for (Size x = 0; x < img.width; x++)
			{
				unsigned char byte = row[x / 8];
				int bit = (byte >> (7 - (x % 8))) & 1;
				unsigned char color = (bit == 1) ? 0 : 255;
				img.data[idx++] = {color, color, color};
			}
		}
	}
	else if (magic == "P5") // Binary Graymap
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			unsigned char v;
			file.read(reinterpret_cast<char *>(&v), 1);
			unsigned char color = scale(v);
			img.data[i] = {color, color, color};
		}
	}
	else if (magic == "P6") // Binary Pixmap
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			unsigned char rgb[3];
			file.read(reinterpret_cast<char *>(rgb), 3);
			img.data[i] = {scale(rgb[0]), scale(rgb[1]), scale(rgb[2])};
		}
	}
	else if (magic == "P7") // PAM
	{
		for (Size i = 0; i < img.width * img.height; i++)
		{
			std::vector<unsigned char> channels(depth);
			file.read(reinterpret_cast<char *>(channels.data()), depth);
			if (depth >= 3)
			{
				img.data[i] = {scale(channels[0]), scale(channels[1]), scale(channels[2])};
			}
			else if (depth == 1 || depth == 2)
			{
				unsigned char color = scale(channels[0]);
				img.data[i] = {color, color, color};
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
		// Write final newline only if the last pixel didn't already get one
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

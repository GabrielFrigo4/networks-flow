#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "src/image.hpp"
#include "src/seeds.hpp"

static void print_usage(const char *program)
{
	std::cout
	    << "Usage:\n"
	    << "  " << program << " -t -i <seeds.txt> -o <seeds.ppm> [-r <image.ppm> | -w <w> -H <h>] [-a | -b]\n"
	    << "  " << program << " -p -i <seeds.ppm> -o <seeds.txt>\n\n"
	    << "Options:\n"
	    << "  -t, --txt2ppm            Convert text seeds to a PPM overlay image\n"
	    << "  -p, --ppm2txt            Convert a PPM overlay image to text seeds\n"
	    << "  -i, --input   <file>     Input file\n"
	    << "  -o, --output  <file>     Output file\n"
	    << "  -r, --reference <file>   Reference PPM to copy dimensions from  (for -t)\n"
	    << "  -w, --width   <val>      Output image width   (for -t, alternative to -r)\n"
	    << "  -H, --height  <val>      Output image height  (for -t, alternative to -r)\n"
	    << "  -a, --ascii              Write output PPM in ASCII format (P3)  [default]\n"
	    << "  -b, --binary             Write output PPM in binary format (P6)\n"
	    << "  -h, --help               Show this message\n";
}

int main(int argc, char *argv[])
{
	bool        txt2ppm        = false;
	bool        ppm2txt        = false;
	std::string input_file;
	std::string output_file;
	std::string reference_file;
	int         width          = 0;
	int         height         = 0;
	bool        ascii          = true;

	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0)
		{
			print_usage(argv[0]);
			return 0;
		}
		else if (std::strcmp(argv[i], "--txt2ppm") == 0 || std::strcmp(argv[i], "-t") == 0)
			txt2ppm = true;
		else if (std::strcmp(argv[i], "--ppm2txt") == 0 || std::strcmp(argv[i], "-p") == 0)
			ppm2txt = true;
		else if ((std::strcmp(argv[i], "--input") == 0 || std::strcmp(argv[i], "-i") == 0) && i + 1 < argc)
			input_file = argv[++i];
		else if ((std::strcmp(argv[i], "--output") == 0 || std::strcmp(argv[i], "-o") == 0) && i + 1 < argc)
			output_file = argv[++i];
		else if ((std::strcmp(argv[i], "--reference") == 0 || std::strcmp(argv[i], "-r") == 0) && i + 1 < argc)
			reference_file = argv[++i];
		else if ((std::strcmp(argv[i], "--width") == 0 || std::strcmp(argv[i], "-w") == 0) && i + 1 < argc)
			width = std::atoi(argv[++i]);
		else if ((std::strcmp(argv[i], "--height") == 0 || std::strcmp(argv[i], "-H") == 0) && i + 1 < argc)
			height = std::atoi(argv[++i]);
		else if (std::strcmp(argv[i], "--ascii") == 0 || std::strcmp(argv[i], "-a") == 0)
			ascii = true;
		else if (std::strcmp(argv[i], "--binary") == 0 || std::strcmp(argv[i], "-b") == 0)
			ascii = false;
	}

	if ((!txt2ppm && !ppm2txt) || (txt2ppm && ppm2txt))
	{
		std::cerr << "error: specify either --txt2ppm or --ppm2txt\n";
		print_usage(argv[0]);
		return 1;
	}

	if (input_file.empty() || output_file.empty())
	{
		std::cerr << "error: --input and --output are required\n";
		print_usage(argv[0]);
		return 1;
	}

	if (txt2ppm)
	{
		const std::vector<Seed> seeds = read_seeds(input_file);

		Size out_width  = 0;
		Size out_height = 0;

		if (!reference_file.empty())
		{
			const Image ref = read_ppm(reference_file);
			out_width       = ref.width;
			out_height      = ref.height;
		}
		else if (width > 0 && height > 0)
		{
			out_width  = static_cast<Size>(width);
			out_height = static_cast<Size>(height);
		}
		else
		{
			std::cerr << "error: --reference or both --width and --height are required for --txt2ppm\n";
			return 1;
		}

		Image seed_img;
		seed_img.width  = out_width;
		seed_img.height = out_height;
		seed_img.data.resize(out_width * out_height, Pixel{0, 255, 0});

		for (const auto &seed : seeds)
		{
			if (seed.x >= out_width || seed.y >= out_height)
				continue;
			const Size idx = seed_img.index(seed.x, seed.y);
			if (seed.label == SeedLabel::FOREGROUND)
				seed_img.data[idx] = {255, 0, 0};
			else if (seed.label == SeedLabel::BACKGROUND)
				seed_img.data[idx] = {0, 0, 255};
		}

		write_ppm(output_file, seed_img, ascii);
		std::cout << "Successfully converted " << input_file << " -> " << output_file
		          << " (PPM seeds, " << out_width << "x" << out_height
		          << ", format: " << (ascii ? "P3 ASCII" : "P6 binary") << ")\n";
	}
	else
	{
		const std::vector<Seed> seeds = read_seeds_from_ppm(input_file);

		std::ofstream out(output_file);
		if (!out.is_open())
		{
			std::cerr << "error: cannot create output file '" << output_file << "'\n";
			return 1;
		}

		out << "# Seeds converted from PPM: " << input_file << "\n";
		out << "# Format: x y F|B\n";
		for (const auto &seed : seeds)
		{
			const char label = (seed.label == SeedLabel::FOREGROUND) ? 'F' : 'B';
			out << seed.x << " " << seed.y << " " << label << "\n";
		}

		std::cout << "Successfully converted " << input_file << " -> " << output_file
		          << " (text seeds, " << seeds.size() << " seeds found)\n";
	}

	return 0;
}

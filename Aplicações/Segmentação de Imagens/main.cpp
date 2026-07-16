#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

#include "src/segmentation.hpp"

static void print_usage(const char *program)
{
	std::cout
	    << "Usage:\n"
	    << "  " << program
	    << " -i <image.ppm> -s <seeds.txt> -o <out.ppm> [-a | -b]\n\n"
	    << "Options:\n"
	    << "  -i, --input    <file>   Input PPM image (P3 or P6)\n"
	    << "  -s, --seeds    <file>   Seeds file (x y F|B per line)\n"
	    << "  -o, --output   <file>   Output PPM image\n"
	    << "                          [default: output_segmented.ppm]\n"
	    << "  -S, --sigma    <value>  Color sensitivity parameter  [default: 50.0]\n"
	    << "  -a, --ascii             Force output PPM in ASCII format (P3)\n"
	    << "  -b, --binary            Force output PPM in binary format (P6)\n"
	    << "  -h, --help              Show this message\n";
}

int main(int argc, char *argv[])
{
	std::string input_file;
	std::string seeds_file;
	std::string output_file = "output_segmented.ppm";
	double sigma = 50.0;
	bool ascii_forced = false;
	bool binary_forced = false;

	if (argc == 1)
	{
		std::cout << "Input image (.ppm): ";
		std::getline(std::cin, input_file);

		std::cout << "Seeds file (.txt): ";
		std::getline(std::cin, seeds_file);

		std::string tmp;
		std::cout << "Output image [output_segmented.ppm]: ";
		std::getline(std::cin, tmp);
		if (!tmp.empty())
			output_file = tmp;

		std::cout << "Sigma [50.0]: ";
		std::getline(std::cin, tmp);
		if (!tmp.empty())
			sigma = std::stod(tmp);

		std::cout << "Output PPM format (auto/ascii/binary) [auto]: ";
		std::getline(std::cin, tmp);
		if (!tmp.empty())
		{
			if (tmp == "ascii")
				ascii_forced = true;
			else if (tmp == "binary")
				binary_forced = true;
		}
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string_view arg(argv[i]);
			if (arg == "--help" || arg == "-h")
			{
				print_usage(argv[0]);
				return 0;
			}
			else if (arg == "--input" || arg == "-i")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "error: missing value for option " << arg << "\n";
					return 1;
				}
				input_file = argv[++i];
			}
			else if (arg == "--seeds" || arg == "-s")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "error: missing value for option " << arg << "\n";
					return 1;
				}
				seeds_file = argv[++i];
			}
			else if (arg == "--output" || arg == "-o")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "error: missing value for option " << arg << "\n";
					return 1;
				}
				output_file = argv[++i];
			}
			else if (arg == "--sigma" || arg == "-S")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "error: missing value for option " << arg << "\n";
					return 1;
				}
				sigma = std::atof(argv[++i]);
			}
			else if (arg == "--ascii" || arg == "-a")
			{
				ascii_forced = true;
			}
			else if (arg == "--binary" || arg == "-b")
			{
				binary_forced = true;
			}
			else
			{
				std::cerr << "error: unknown option " << arg << "\n";
				print_usage(argv[0]);
				return 1;
			}
		}
	}

	if (input_file.empty() || seeds_file.empty())
	{
		std::cerr << "error: --input and --seeds are required\n";
		print_usage(argv[0]);
		return 1;
	}

	std::cout << "[1/4] Loading image and seeds...\n";
	const Image img = read_ppm(input_file);
	const auto seeds = read_seeds(seeds_file);

	std::cout << "[2/4] Building segmentation graph...\n";
	auto graph = build_graph(img, seeds, sigma);
	const Size num_pixels = img.width * img.height;

	std::cout << "[3/4] Computing max-flow / min-cut...\n";
	const Long max_flow = graph.network->compute_max_flow(graph.source, graph.sink);
	std::cout << "      max-flow = " << max_flow << "\n";

	std::cout << "[4/4] Generating segmented image...\n";
	const auto fg_mask = extract_foreground_mask(graph, num_pixels);
	const Image result = apply_mask(img, fg_mask);

	bool output_ascii = img.is_ascii;
	if (ascii_forced)
		output_ascii = true;
	else if (binary_forced)
		output_ascii = false;

	write_ppm(output_file, result, output_ascii);

	std::cout << "Done -> " << output_file << "\n";
	return 0;
}

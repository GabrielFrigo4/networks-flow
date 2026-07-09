#include <cstring>
#include <iostream>
#include <string>

#include "src/segmentation.hpp"

static void print_usage(const char *program)
{
	std::cout
	    << "Usage:\n"
	    << "  " << program
	    << " --input <image.ppm> --seeds <seeds.txt> --output <out.ppm>\n\n"
	    << "Options:\n"
	    << "  --input   <file>   Input PPM image (P3 or P6)\n"
	    << "  --seeds   <file>   Seeds file (x y F|B per line)\n"
	    << "  --output  <file>   Output PPM image  [default: output_segmented.ppm]\n"
	    << "  --sigma   <value>  Color sensitivity parameter  [default: 30.0]\n"
	    << "  --help             Show this message\n";
}

int main(int argc, char *argv[])
{
	std::string input_file;
	std::string seeds_file;
	std::string output_file = "output_segmented.ppm";
	double sigma = 30.0;

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

		std::cout << "Sigma [30.0]: ";
		std::getline(std::cin, tmp);
		if (!tmp.empty())
			sigma = std::stod(tmp);
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (std::strcmp(argv[i], "--help") == 0 ||
			    std::strcmp(argv[i], "-h") == 0)
			{
				print_usage(argv[0]);
				return 0;
			}
			else if (std::strcmp(argv[i], "--input") == 0 && i + 1 < argc)
				input_file = argv[++i];
			else if (std::strcmp(argv[i], "--seeds") == 0 && i + 1 < argc)
				seeds_file = argv[++i];
			else if (std::strcmp(argv[i], "--output") == 0 && i + 1 < argc)
				output_file = argv[++i];
			else if (std::strcmp(argv[i], "--sigma") == 0 && i + 1 < argc)
				sigma = std::atof(argv[++i]);
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
	write_ppm(output_file, result);

	std::cout << "Done -> " << output_file << "\n";
	return 0;
}

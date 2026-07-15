#include <iostream>
#include <string>

#include "src/image.hpp"

static void print_usage(const char *program)
{
	std::cout
	    << "Usage:\n"
	    << "  " << program << " -i <input.ppm> -o <output.ppm> [-a | -b]\n\n"
	    << "Options:\n"
	    << "  -i, --input   <file>   Input Netpbm image (P1–P7)\n"
	    << "  -o, --output  <file>   Output Netpbm image\n"
	    << "  -a, --ascii            Force output in ASCII format (P3)\n"
	    << "                         [default: match input]\n"
	    << "  -b, --binary           Force output in binary format (P6)\n"
	    << "  -h, --help             Show this message\n";
}

int main(int argc, char *argv[])
{
	std::string input_file;
	std::string output_file;
	bool ascii_forced = false;
	bool binary_forced = false;

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
		else if (arg == "--output" || arg == "-o")
		{
			if (i + 1 >= argc)
			{
				std::cerr << "error: missing value for option " << arg << "\n";
				return 1;
			}
			output_file = argv[++i];
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

	if (input_file.empty() || output_file.empty())
	{
		std::cerr << "error: --input and --output are required\n";
		print_usage(argv[0]);
		return 1;
	}

	Image img = read_ppm(input_file);

	bool to_ascii = img.is_ascii;
	if (ascii_forced)
		to_ascii = true;
	else if (binary_forced)
		to_ascii = false;

	write_ppm(output_file, img, to_ascii);

	std::cout << "Converted '" << input_file << "' -> '" << output_file
	          << "' (" << (to_ascii ? "P3 ASCII" : "P6 binary") << ")\n";
	return 0;
}

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
	    << "  -a, --ascii            Force output in ASCII format (P3)  [default: match input]\n"
	    << "  -b, --binary           Force output in binary format (P6)\n"
	    << "  -h, --help             Show this message\n";
}

int main(int argc, char *argv[])
{
	std::string input_file;
	std::string output_file;
	bool        ascii_forced  = false;
	bool        binary_forced = false;

	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0)
		{
			print_usage(argv[0]);
			return 0;
		}
		else if ((std::strcmp(argv[i], "--input") == 0 || std::strcmp(argv[i], "-i") == 0) && i + 1 < argc)
			input_file = argv[++i];
		else if ((std::strcmp(argv[i], "--output") == 0 || std::strcmp(argv[i], "-o") == 0) && i + 1 < argc)
			output_file = argv[++i];
		else if (std::strcmp(argv[i], "--ascii") == 0 || std::strcmp(argv[i], "-a") == 0)
			ascii_forced = true;
		else if (std::strcmp(argv[i], "--binary") == 0 || std::strcmp(argv[i], "-b") == 0)
			binary_forced = true;
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

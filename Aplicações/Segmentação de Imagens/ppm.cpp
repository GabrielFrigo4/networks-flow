#include <iostream>
#include <string>

#include "src/image.hpp"

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <input.ppm> <output.ppm> <ascii|binary>\n";
		return 1;
	}

	std::string input = argv[1];
	std::string output = argv[2];
	std::string format = argv[3];

	bool to_ascii = false;
	if (format == "ascii" || format == "P3")
		to_ascii = true;
	else if (format == "binary" || format == "P6")
		to_ascii = false;
	else
	{
		std::cerr << "Error: Format must be 'ascii' or 'binary'.\n";
		return 1;
	}

	Image img = read_ppm(input);
	write_ppm(output, img, to_ascii);

	std::cout << "Successfully converted '" << input << "' to " << format << " format in '" << output << "'.\n";
	return 0;
}

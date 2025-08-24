#include "file_to_string.hpp"

#include <fstream>

std::string file_to_string(const std::string& filename) {
	std::ifstream infile(filename);
	std::string text;
	text.assign( (std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()) );
	return text;
}
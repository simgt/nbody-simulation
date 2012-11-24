#include "cl.hh"
#include <fstream>

char* loadSource (const char* path) {
	std::ifstream t (path);
	
	t.seekg(0, std::ios::end);
	int size = t.tellg();
	t.seekg(0, std::ios::beg);

	char* str = new char[size + 1];

	t.read(str, size);
	str[size] = '\0';

	return str;
}
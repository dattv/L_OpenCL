#include "error.h"

void check_error(cl_int err, const char *c, const char *filename, int line) {
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << c << std::endl;
		std::cout << "Error in file:" << filename << std::endl;
		std::cout << "Error in line: " << line << std::endl;
		return;
	}
}
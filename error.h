#pragma once
#ifndef ERROR_H

#include <iostream>
#include <CL/cl.h>

void check_error(cl_int err, const char *c, const char *filename, int line) {
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << c << std::endl;
		std::cout << "Error in file:" << filename << std::endl;
		std::cout << "Error in line: " << line << std::endl;
		return;
	}
}

#define checkError(E, S) check_error(E, S, __FILE__, __LINE__)
#endif // !ERROR_H




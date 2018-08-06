#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "error.h"

#define LENGHT (1024)
#define MAX_SOURCE_SIZE (0x1000000)

int main() {

	cl_int err; // error code returned form OpenCL calls

	float *A = (float*)malloc(LENGHT * sizeof(float));
	float *B = (float*)malloc(LENGHT * sizeof(float));
	float *C = (float*)malloc(LENGHT * sizeof(float));


	// fill vectors A, and B with random float number
	for (int i = 0; i < LENGHT; i++) {
		A[i] = rand() / (float)RAND_MAX;
		B[i] = rand() / (float)RAND_MAX;
	}

	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("vector_add_kernel.cl", "r");
	if (!fp) {
		printf("falled to load kernel\n");
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE*sizeof(char));
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// discovery plaotform and GPU device
	cl_uint numPlatflorm;
	err = clGetPlatformIDs(0, NULL, &numPlatflorm);
	checkError(err, "Getting number of platform");
	printf("Number of Platform: %d", numPlatflorm);

	cl_platform_id *platforms;
	platforms = (cl_platform_id*)malloc(numPlatflorm * sizeof(cl_platform_id));
	err = clGetPlatformIDs(numPlatflorm, platforms, NULL);
	checkError(err, "Getting platforms");
	
	cl_device_id deviceID;
	err = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_ALL, 1, &deviceID, NULL);
	checkError(err, "Getting device id");

	// create a compute context
	cl_context context = clCreateContext(0, 1, &deviceID, NULL, NULL, &err);
	checkError(err, "Getting context");

	// Create command queue
	cl_command_queue command_queue = clCreateCommandQueue(context, deviceID, 0, &err);
	checkError(err, "Getting command queue");
	
	// Create the comptue program form the source buffer
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)& source_str, (const size_t*)& source_size, &err);
	checkError(err, "Creating program");

	// Build program 
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	checkError(err, "Building program");

	// Create the comptue kernel from the program
	cl_kernel kernel = clCreateKernel(program, "vadd", &err);
	checkError(err, "\nCreating Kernel");

	
	getchar();
	return 0;
}
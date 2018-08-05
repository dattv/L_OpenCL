#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int main(void) {

	// Discovery number of platforms
	cl_device_id device_id = NULL;
	cl_uint ret_num_device;
	cl_uint ret_num_platform;

	cl_int err;

	err = clGetPlatformIDs(0, NULL, &ret_num_platform);
	if (err == 0) {
		std::cout << "Number of OpenCL platform :"
			<< ret_num_platform << std::endl;
	}

	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platform * sizeof(cl_platform_id));

	err = clGetPlatformIDs(ret_num_platform, platforms, NULL);
	checkError(err, "Getting Platform");

	printf("Number of OpenCL platforms: %d\n", ret_num_platform);

	// Investigate each platform
	for (cl_uint i = 0; i < ret_num_platform; i++) {
		cl_char string[1024] = { 0 };
		//print out platforms name
		
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(string), &string, NULL);
		checkError(err, "Getting Platform Name");
		printf("Platform name: %s\n", string);

		// Print out platform vendor
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(string), &string, NULL);
		checkError(err, "Getting platform vendor");
		printf("Platform vendor Name: %s\n", string);

		// Print out the platform OpenCL version
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(string), &string, NULL);
		checkError(err, "Getting OpenCL version", string);
		printf("Platform version: %s\n", string);

		// Count number device
		cl_uint ret_num_device;
		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &ret_num_device);
		checkError(err, "Getting number of Device");
		printf("Number of device: %d\n", ret_num_device);

		cl_device_id *device;
		device = (cl_device_id*)malloc(ret_num_device * sizeof(cl_device_id));
		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, ret_num_device, device, NULL);
		checkError(err, "Getting device");
		
		// Investigate device
		for (cl_uint j = 0; j < ret_num_device; j++) {
			// Get device name
			err = clGetDeviceInfo(device[j], CL_DEVICE_NAME, sizeof(string), &string, NULL);
			checkError(err, "Getting device name");
			printf("\t\tName: %s\n", string);

			// Get device OpenCL version
			err = clGetDeviceInfo(device[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(string), &string, NULL);
			checkError(err, "Getting device OpenCL C version");
			printf("\t\tVersion: %s\n", string);

			// Get Max. Compute units
			cl_uint num;
			err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num, NULL);
			checkError(err, "Getting devicie max compute units");
			printf("\t\tMax. Compute Units: %d\n", num);

			// Get local memory
			cl_ulong mem_size;
			err = clGetDeviceInfo(device[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
			checkError(err, "Getting device local memory size");
			printf("\t\tLocal Memory Size: %llu KB\n", mem_size / 1024);

			// Get global memory
			err = clGetDeviceInfo(device[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
			checkError(err, "Getting device global memory");
			printf("\t\tGlobal Memory Size: %llu MB\n", mem_size / (1024 * 1024));

			// Get maximum buffer alloc. size
			err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &mem_size, NULL);
			checkError(err, "Getting devicie max alocation size");
			printf("\t\tMax Alloc size: %llu MB\n", mem_size / (1024 * 1024));

			// Get work-group size information
			size_t size;
			err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &size, NULL);
			checkError(err, "Getting devicie max work-group size");
			printf("\t\tMax Work-group Total Size: %ld\n", size);

			// Find the maximum dimension of the work-group
			err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num, NULL);
			checkError(err, "Getting device max work-item dims");
			
			// Get max. dimension of the work-groups
			size_t *dims;
			dims = (size_t*)malloc(num * sizeof(size_t));
			err = clGetDeviceInfo(device[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dims)*num, &dims[0], NULL);
			checkError(err, "Getting device mas work-item sizes");
			printf("\t\tMax Work-group Dims: ( ");
			for (size_t k = 0; k < num; k++) {
				printf("%ld ", dims[k]);
			}
			printf(")\n");
			free(dims);
		}
	}

	getchar();
	return 0;
}
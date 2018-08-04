#include <stdio.h>
#include <stdlib.h>

#ifdef _APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x1000000)

int main() {
	
	puts("started running \n");

	// Create the two input vectors
	int i;
	const int LIST_SIZE = 1024;
	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *B = (int*)malloc(sizeof(int)*LIST_SIZE);

	for (int i = 0; i < LIST_SIZE; i++) {
		A[i] = i;
		B[i] = LIST_SIZE - i;
	}

	//load the kernel sourc ecode into the array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("vector_add_kernel.cl", "r");
	if (!fp) {
		puts("faled to load kernel\n");
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);

	fclose(fp);

	puts("kernel loading done\n");

	// Get platform adn device information
	cl_device_id device_id = NULL;
	cl_uint ret_num_device;
	cl_uint ret_num_platform;

	cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platform);
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platform * sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platform, platforms, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_device);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Create and OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	printf("ret at %d, is %d\n", __LINE__, ret);

	// Create a command queue
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("ret at %d, is %d\n", __LINE__, ret);

	// Create memory buffer ont he device for each vector
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

	// Copy list A, B to their respective memory
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

	printf("before bulding\n");
	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source_str, (const size_t*)&source_size, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	printf("after bulding\n");
	//Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Set the argument of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

	printf("before execution\n");
	// Execute the OpenCL kernel on the list
	size_t global_item_size = LIST_SIZE;
	size_t local_item_size = 64;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	printf("after Execution\n");

	// Read the memory buffer C on the device to the local variable C
	int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

	printf("after copying\n");
	// Display the result on the screen
	for (int i = 0; i < LIST_SIZE; i++) {
		printf("%d + %d = %d\n", A[i], B[i], C[i]);
	}

	// Clearn up
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseMemObject(c_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(A);
	free(B);
	free(C);
	getchar();
	return 0;
}
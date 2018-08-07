#include "discovery_platforms.h"

void displayPlatform() {

	cl_uint numOfPlatform;
	cl_int err;

	err = clGetPlatformIDs(0, NULL, &numOfPlatform);
	checkError(err, "Getting number of platform");

	cl_platform_id *platformID = (cl_platform_id*)malloc(numOfPlatform * sizeof(cl_platform_id));
	printf("Number of Platfomr: %d", numOfPlatform);

	clGetPlatformIDs(numOfPlatform, platformID, NULL);

	for (cl_uint i = 0; i < numOfPlatform; i++) {

		displayPlatformInfo(platformID[i], CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");

		displayPlatformInfo(platformID[i], CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");

		displayPlatformInfo(platformID[i], CL_PLATFORM_NAME, "CL_PLATFORM_NAME");

		displayPlatformInfo(platformID[i], CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");

		displayPlatformInfo(platformID[i], CL_PLATFORM_EXTENSIONS, "CL_PLATFORM_EXTENSIONS");

	}
}
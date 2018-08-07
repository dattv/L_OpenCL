#include "utility.h"

void displayPlatformInfo(cl_platform_id platformID, cl_platform_info platformInfo, const char* str) {
	cl_int err = 0;
	size_t size = 0;

	err = clGetPlatformInfo(platformID, platformInfo, 0, NULL, &size);
	checkError(err, "Getting platform info");

	char * moreInfo = (char*)malloc(size * sizeof(char));
	err = clGetPlatformInfo(platformID, platformInfo, size, moreInfo, NULL);
	checkError(err, "Getting more info");

	printf("%s:\t%s\n", str, moreInfo);
	return;

}
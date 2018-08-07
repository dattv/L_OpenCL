#pragma once
#ifndef UTILITY_H
#define	UTILITY_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "error.h"

void displayPlatformInfo(cl_platform_id platformID, cl_platform_info platformInfo, const char* str);
#endif // !UTILITY_H


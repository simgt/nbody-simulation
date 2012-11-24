#pragma once

extern "C" {
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
}

/**
 * return a null terminated string with the content of the indicated file
 */
char* loadSource (const char* path);
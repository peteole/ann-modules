/*
 * openclTools.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: olep
 */
#include "openclTools.h"

namespace std{
void initializeOpencl(){
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("src/vector_add_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }

    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
    printf("kernel loading done\n");
    // Get platform and device information
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;


    cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
    cl_platform_id *platforms = NULL;
    platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

    ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
    printf("ret at %d is %d\n", __LINE__, ret);

    ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1,
            &device_id, &ret_num_devices);
    printf("ret at %d is %d\n", __LINE__, ret);
}
}

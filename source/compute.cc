#include "cl.hh"
#include "common.hh"

#include <iostream>

void compute (
		cl_device_id device,
		cl_context context,
		cl_kernel kernel,
		cl_command_queue queue,
		cl_mem p_buffer,
		float dt
	) {
	cl_int error;
	uint count = PARTICLES_COUNT;
	
	// set kernel's arguments
    error = 0;
    error  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &p_buffer);
    error |= clSetKernelArg(kernel, 1, sizeof(uint), &count);
    error |= clSetKernelArg(kernel, 2, sizeof(float), &dt);
    if (error != CL_SUCCESS) {
        std::cerr << "error: Failed to set kernel arguments! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
 
    // get the maximum work group size for executing the kernel on the device
    size_t local;
    error = clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, 0);
    if (error != CL_SUCCESS) {
        std::cerr << "error: Failed to retrieve kernel work group info! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
 
    // execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    size_t global = count;
    if (global < local)
        local = global;

    error = clEnqueueNDRangeKernel(
                queue, kernel, 1,
                0, &global, &local,
                0, 0, 0
            );
    if (error) {
        std::cerr << "error: Failed to execute kernel! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
}
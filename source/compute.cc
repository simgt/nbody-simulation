#include "cl.hh"
#include "common.hh"

#include <glte/glte.hh>
#include <iostream>

void compute (
		cl_device_id device,
		cl_context context,
		cl_kernel kernel,
		cl_command_queue queue,
        cl_mem p_buffer,
        cl_mem q_buffer,
        cl_mem u_buffer,
        cl_mem v_buffer,
		float dt
	) {
	cl_int error;
	
	// set kernel's arguments
    error = 0;
    error  = clSetKernelArg(kernel, 0, sizeof(float), &dt);
    error |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &p_buffer);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &q_buffer);
    error |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &u_buffer);
    error |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &v_buffer);
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
    size_t global = PARTICLES_COUNT;
    if (global < local)
        local = global;

    glFinish();
    clEnqueueAcquireGLObjects(queue, 1, &p_buffer, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer, 0, 0, 0);

    error = clEnqueueNDRangeKernel(
                queue, kernel, 1,
                0, &global, &local,
                0, 0, 0
            );

    clEnqueueReleaseGLObjects(queue, 1, &p_buffer, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer, 0, 0, 0);
    clFinish(queue);

    if (error) {
        std::cerr << "error: Failed to execute kernel! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
}
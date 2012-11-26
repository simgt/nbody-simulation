#include "cl.hh"
#include "common.hh"

#include <glte/glte.hh>
#include <iostream>

void nbody_compute (
		cl_device_id device,
		cl_context context,
		cl_kernel kernel,
		cl_command_queue queue,
        cl_mem p_buffer,
        cl_mem q_buffer,
		float dt
	) {
	const float eps = 0.001;
	uint nburst = std::ceil(dt / eps);
	dt /= nburst;

	size_t global = PARTICLES_COUNT;
	size_t local = 256;

	if (local > global)
		local = global;

	// set kernel's arguments
    cl_int error = 0;
    error  = clSetKernelArg(kernel, 0, sizeof(float), &dt);
    error |= clSetKernelArg(kernel, 1, sizeof(float), &eps);
    error |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &p_buffer);
    error |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &q_buffer);
    error |= clSetKernelArg(kernel, 4, sizeof(cl_float4) * local, 0);
    if (error != CL_SUCCESS) {
        std::cerr << "error: Failed to set kernel arguments! " << error << std::endl;
        exit(EXIT_FAILURE);
    }

    glFinish();
    clEnqueueAcquireGLObjects(queue, 1, &p_buffer, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer, 0, 0, 0);

    for (uint i = 0; i < nburst; i++) {
		error = clEnqueueNDRangeKernel(
					queue, kernel, 1,
					0, &global, &local,
					0, 0, 0
				);
	}

    clEnqueueReleaseGLObjects(queue, 1, &p_buffer, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer, 0, 0, 0);
    clFinish(queue);

    if (error) {
        std::cerr << "error: Failed to execute kernel! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
}
#include "common.hh"
#include "cl.hh"
#include "nbody.hh"

#include <glte/glte.hh>
#include <iostream>

int main () {
    // OpenGL init 
    te::Context context(WINDOW_W, WINDOW_H, false, "particles.cc");
    context.clear_color({0, 0, 0, 1});

    // OpenCL init

    cl_int error;
    cl_device_id device;
    error = clGetDeviceIDs(0, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    if (error != CL_SUCCESS) {
        std::cerr << "error: Failed to create a device group!" << std::endl;
        return EXIT_FAILURE;
    }
  
    // create OpenCL context from OpenGL context 
    cl_context clcontext = 0;
#if  defined(__APPLE__) || defined(__MACOSX)
    {
        CGLContextObj glcontext = CGLGetCurrentContext();
        CGLShareGroupObj share_group = CGLGetShareGroup(glcontext);

        cl_context_properties properties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties)share_group,
            0
        };

        clcontext = clCreateContext(properties, 1, &device, 0, 0, &error);
    }
#else
    std::cerr << "error: No way known to link OpenCL and OpenGL !"
    clcontext = clCreateContext(0, 1, &device, 0, 0, &error);
    exit(EXIT_FAILURE);
#endif

    if (!clcontext) {
        std::cerr << "error: Failed to create a compute context!" << std::endl;
        return EXIT_FAILURE;
    }

    // create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(clcontext, device, 0, &error);
    if (!command_queue) {
        std::cerr << "error: Failed to create a command queue!" << std::endl;
        return EXIT_FAILURE;
    }

    // simulation init
    NBody nbody(device, clcontext, 4096);
    nbody.reset();

    // camera
    Vec4f camera (0, 0, 1, 0);

    // main loop
    while (context.exist()) {
        if (glfwGetKey('A')) // zoom in
            camera.z() = std::max(0.1, camera.z() * 0.9);
        if (glfwGetKey('Z')) // zoom out
            camera.z() *= 1.1;

        if (glfwGetKey(GLFW_KEY_UP))
            camera.y() += 0.1 * camera.z();
        if (glfwGetKey(GLFW_KEY_DOWN))
            camera.y() -= 0.1 * camera.z();
        if (glfwGetKey(GLFW_KEY_LEFT))
            camera.x() -= 0.1 * camera.z();
        if (glfwGetKey(GLFW_KEY_RIGHT))
            camera.x() += 0.1 * camera.z();

        context.clear();
        float dt = context.time();
        //float fps = 1 / t;

        nbody.iterate(command_queue, dt / 200);
        nbody.draw(camera);

        context.swap();
    }


    // OpenCL release

    clReleaseCommandQueue(command_queue);
    clReleaseContext(clcontext);

    return 0;
}
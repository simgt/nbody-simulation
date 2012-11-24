#include "common.hh"
#include "cl.hh"
#include "particle_mesh.hh"
#include <glte/glte.hh>

#include <iostream>
#include <cstdlib>

void compute (
        cl_device_id device,
        cl_context context,
        cl_kernel kernel,
        cl_command_queue,
        cl_mem p_buffer,
        float dt
    );

void draw (
        te::Context& context,
        te::Shader& shader,
        ParticleMesh& mesh
    );

int main () {
    // ------------
    //    OPENGL 
    // ------------

    te::Context context(WINDOW_W, WINDOW_H, false, "particles.cc");

    context.clear_color({0.2, 0.2, 0.2, 1});

    te::Shader* shader = te::Shader::load(
                                    "solid.Vertex",
                                    "solid.Fragment"
                                );

    ParticleMesh* mesh = new ParticleMesh;
    mesh->vertex_buffer()->reserve(PARTICLES_COUNT);

    // ------------
    //    OPENCL 
    // ------------

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
 
    // create the compute program from the source buffer
    const char* kernel_source = loadSource("runtime/hello.cl");
    cl_program program = clCreateProgramWithSource(clcontext, 1, &kernel_source, 0, &error);
    if (!program) {
        std::cerr << "error: Failed to create compute program!" << std::endl;
        return EXIT_FAILURE;
    }
 
    // build the program executable
    error = clBuildProgram(program, 0, 0, 0, 0, 0);
    if (error != CL_SUCCESS) {
        size_t len;
        char buffer[2048];
 
        std::cerr << "error: Failed to build program executable!" << std::endl;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cerr << buffer << std::endl;
        exit(1);
    }
 
    // create the compute kernel in the program
    cl_kernel kernel = clCreateKernel(program, "position", &error);
    if (!kernel || error != CL_SUCCESS) {
        std::cerr << "error: Failed to create compute kernel!" << std::endl;
        exit(1);
    }

    // create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(clcontext, device, 0, &error);
    if (!command_queue) {
        std::cerr << "error: Failed to create a command queue!" << std::endl;
        return EXIT_FAILURE;
    }
 
    // create OpenCL buffer from the OpenGL vertex buffer
    cl_mem p_buffer = clCreateFromGLBuffer(
                            clcontext,
                            CL_MEM_READ_WRITE,
                            mesh->vertex_buffer()->handle(),
                            &error
                        );
    if (!p_buffer) {
        std::cerr << "error: Failed to allocate device memory!" << std::endl;
        exit(1);
    }

    // initialize position and velocity buffers
    Vec2f p_data[PARTICLES_COUNT];
    for (uint i = 0; i < PARTICLES_COUNT; i++) {
        p_data[i] = Vec2f::Random();
        p_data[i].normalize();
    }

    error = clEnqueueWriteBuffer(command_queue, p_buffer, CL_TRUE, 0, sizeof(Vec2f) * PARTICLES_COUNT, p_data, 0, 0, 0);
    if (error != CL_SUCCESS) {
        std::cerr << "Error: Failed to write to source array! " << error << std::endl;
        exit(1);
    }

    // -------------
    //    LOOPING
    // -------------

    while (context.exist()) {
        float dt = context.time();
        //float fps = 1 / t;

        compute(device, clcontext, kernel, command_queue, p_buffer, dt);
        draw(context, *shader, *mesh);
    }

    // ------------
    //    OPENCL 
    // ------------

    clReleaseMemObject(p_buffer);

    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(clcontext);

    return 0;
}
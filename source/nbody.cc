#include "nbody.hh"

#include <glte/glte.hh>
#include <iostream>
#include <vector>

NBody::NBody (
        const cl_device_id device,
        const cl_context clcontext,
        uint size
    )
    : size_ (size) {

    // OpenGL objects
    p_mesh_ = new Mesh;
    q_mesh_ = new Mesh;
    p_mesh_->vertex_buffer()->reserve(size);
    q_mesh_->vertex_buffer()->reserve(size);

    shader_ = te::Shader::load(
                    "solid.Vertex",
                    "solid.Fragment"
                );


    // OpenCL objects
    cl_int error;

    // create the compute program from source file
    const char* kernel_source = loadSource("runtime/nbody.cl");
    cl_program program_ = clCreateProgramWithSource(clcontext, 1, &kernel_source, 0, &error);
    if (!program_) {
        std::cerr << "error: Failed to create compute program!" << std::endl;
        exit(EXIT_FAILURE);
    }
 
    // build the program executable
    error = clBuildProgram(program_, 0, 0, 0, 0, 0);
    if (error != CL_SUCCESS) {
        size_t len;
        char buffer[2048];
 
        std::cerr << "error: Failed to build program executable!" << std::endl;
        clGetProgramBuildInfo(program_, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cerr << buffer << std::endl;
        exit(1);
    }

    delete[] kernel_source;
 
    // create the compute kernel in the program
    kernel_ = clCreateKernel(program_, "iterate", &error);
    if (!kernel_ || error != CL_SUCCESS) {
        std::cerr << "error: Failed to create compute kernel!" << std::endl;
        exit(1);
    }

    // create OpenCL buffer from the OpenGL vertex buffer
    p_buffer_ = clCreateFromGLBuffer(
                            clcontext, CL_MEM_READ_WRITE,
                            p_mesh_->vertex_buffer()->handle(),
                            &error
                        );

    q_buffer_ = clCreateFromGLBuffer(
                            clcontext, CL_MEM_READ_WRITE,
                            q_mesh_->vertex_buffer()->handle(),
                            &error
                        );
    
    if (!p_buffer_ || !q_buffer_) {
        std::cerr << "error: Failed to create buffers!" << std::endl;
        exit(1);
    }
}

NBody::~NBody () {
    // OpenCL
    clReleaseMemObject(p_buffer_);
    clReleaseMemObject(q_buffer_);
    clReleaseProgram(program_);
    clReleaseKernel(kernel_);

    // OpenGL
    delete p_mesh_;
    delete q_mesh_;
    delete shader_;
}

void NBody::reset () {
    // initialize position and velocity buffers
    std::vector<Particle> p_data (size_);

    //p_data[0].position = Vec4f(0, 0, 0, 50);
    //p_data[0].velocity = Vec4f::Zero();

    for (uint i = 0; i < size_; i++) {
        //Vec2f p = Vec2f::Random().normalized().array() / 2;
        p_data[i].position.x() = (float)rand() / RAND_MAX * 2 - 1;//p.x();
        p_data[i].position.y() = (float)rand() / RAND_MAX * 2 - 1;//p.y();
        p_data[i].position.z() = 0;
        p_data[i].position.w() = (float)rand() / RAND_MAX;

        //float k = (float)rand() / RAND_MAX;
        //p_data[i].velocity = p_data[i].position.x() == 0 ? Vec4f(k, 0, 0, 0)
        //                               : Vec4f(k, -p_data[i].position.y() / p_data[i].position.x() * k, 0, 0);
        p_data[i].velocity = Vec4f::Zero();
    }

    p_mesh_->vertex_buffer()->data(p_data);
    q_mesh_->vertex_buffer()->data(p_data);
}

void NBody::iterate (cl_command_queue queue, float dt) {
	const float eps = 0.001;
	uint nburst = std::ceil(dt / eps);
	dt /= nburst;

	size_t global = size_;
	size_t local = 256;

	if (local > global)
		local = global;

    swap_buffers();

	// set kernel's arguments
    cl_int error = 0;
    error  = clSetKernelArg(kernel_, 0, sizeof(float), &dt);
    error |= clSetKernelArg(kernel_, 1, sizeof(float), &eps);
    error |= clSetKernelArg(kernel_, 2, sizeof(cl_mem), &p_buffer_);
    error |= clSetKernelArg(kernel_, 3, sizeof(cl_mem), &q_buffer_);
    error |= clSetKernelArg(kernel_, 4, sizeof(cl_float4) * local, 0);
    if (error != CL_SUCCESS) {
        std::cerr << "error: Failed to set kernel arguments! " << error << std::endl;
        exit(EXIT_FAILURE);
    }

    glFinish();

    clEnqueueAcquireGLObjects(queue, 1, &p_buffer_, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer_, 0, 0, 0);

    for (uint i = 0; i < nburst; i++) {
		error = clEnqueueNDRangeKernel(
					queue, kernel_, 1,
					0, &global, &local,
					0, 0, 0
				);
	}

    clEnqueueReleaseGLObjects(queue, 1, &p_buffer_, 0, 0, 0);
    clEnqueueAcquireGLObjects(queue, 1, &q_buffer_, 0, 0, 0);
    clFinish(queue);

    if (error) {
        std::cerr << "error: Failed to execute kernel! " << error << std::endl;
        exit(EXIT_FAILURE);
    }
}

void NBody::draw () const {
    static float distance = 1;
    static Vec4f camera (0, 0, 1, 0);

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

    glUseProgram(shader_->handle());

    // retrieve window size
    GLuint uniform_viewport_size = glGetUniformLocation(shader_->handle(), "viewport_size");
    int width, height;
    glfwGetWindowSize(&width, &height);
    Vec2f viewport_size(width, height);
    glUniform2fv(uniform_viewport_size, 1, viewport_size.data());

    // set radius uniform
    GLuint uniform_point_radius = glGetUniformLocation(shader_->handle(), "point_radius");
    glUniform1f(uniform_point_radius, PARTICLES_RADIUS);

    // set scale uniform
    GLuint uniform_camera = glGetUniformLocation(shader_->handle(), "camera");
    glUniform4fv(uniform_camera, 1, camera.data());

    // bind the VAO associated to the mesh
    glBindVertexArray(p_mesh_->handle());
    
    CHECK_GL_ERROR();

    // enable/disable attributes depending on the shader
    for (uint i = 0; i < te::Shader::NUM_ATTRIBS; i++) {
        if (shader_->has_attribute(te::Shader::Attribute(i)))
            glEnableVertexAttribArray(i);
        else
            glDisableVertexAttribArray(i);
    }

    CHECK_GL_ERROR();

    // draw mesh's elements
    glPointSize(PARTICLES_RADIUS * 2);
    glDrawArrays(
        GL_POINTS,
        0,
        p_mesh_->vertex_buffer()->size()
    );

    CHECK_GL_ERROR();

    // clear states
    glBindVertexArray(0);
    glUseProgram(0);
}

namespace {
    template <typename T>
    void swap (T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }
}

void NBody::swap_buffers () {
    swap(p_buffer_, q_buffer_);
    swap(p_mesh_, q_mesh_);
}
#pragma once

#include "common.hh"
#include "cl.hh"
#include <glte/glte.hh>

class Mesh;

class NBody {
public:
	NBody (const cl_device_id device, const cl_context clcontext, uint size);
	~NBody ();

	void reset ();
	void iterate (cl_command_queue queue, float dt);
	void draw (const Vec4f camera) const;

private:
	void swap_buffers ();

	uint size_;

	Mesh* p_mesh_;
	Mesh* q_mesh_;
	te::Shader* shader_;
	struct {
		GLuint viewport_size;
		GLuint max_radius;
		GLuint camera;
	} uniforms_;

	cl_mem p_buffer_, q_buffer_;
	cl_program program_;
	cl_kernel kernel_;
};

struct Particle {
	Vec4f position;
	Vec4f velocity;
};

class Mesh {
public:
	typedef te::Buffer<GL_ARRAY_BUFFER, Particle> Buffer;

	Mesh ();
	Mesh (GLenum vertex_hint);
	Mesh (Buffer* vertex_buffer);
	~Mesh ();

	// accessors

	GLuint handle () const { return handle_; }
	Buffer* vertex_buffer () { return vertex_buffer_; }
	const Buffer* vertex_buffer () const { return vertex_buffer_; }

private:
	GLuint handle_;
	Buffer* vertex_buffer_;
};
#pragma once

#include <glte/glte.hh>
#include "common.hh"

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
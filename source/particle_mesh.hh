#pragma once

#include <glte/glte.hh>

class ParticleMesh {
public:
	typedef te::Buffer<GL_ARRAY_BUFFER, Vec2f> Buffer;

	ParticleMesh ();
	ParticleMesh (GLenum vertex_hint);
	ParticleMesh (Buffer* vertex_buffer);
	~ParticleMesh ();

	// accessors

	GLuint handle () const { return handle_; }
	Buffer* vertex_buffer () { return vertex_buffer_; }
	const Buffer* vertex_buffer () const { return vertex_buffer_; }

private:
	GLuint handle_;
	Buffer* vertex_buffer_;
};
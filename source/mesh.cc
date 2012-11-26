#include "mesh.hh"
#include <glte/glte.hh>

#include <cassert>
#include <cstddef>

Mesh::Mesh ()
	: Mesh (GL_STATIC_DRAW) {
}

Mesh::Mesh (GLenum vertex_hint)
	: Mesh (new Buffer(vertex_hint)) {
}

Mesh::Mesh (Buffer* vertex_buffer)
	: vertex_buffer_ (vertex_buffer) {

	assert(vertex_buffer_ != 0);

	// VertexArrays allow to store the vertex and element vertex_buffers ids
	glGenVertexArrays(1, &handle_);
	glBindVertexArray(handle_);
	
	// bind the vertex and element buffers (if any)
	glBindBuffer(te::VertexBuffer::Target, vertex_buffer_->handle());
	
	// setup attributes
	glVertexAttribPointer(
			te::Shader::ATTRIB_POSITION,
			4, GL_FLOAT,
			GL_FALSE,
			sizeof(Particle),
			reinterpret_cast<void*>(offsetof(Particle, position))
		);

	glVertexAttribPointer(
			te::Shader::ATTRIB_NORMAL,
			4, GL_FLOAT,
			GL_FALSE,
			sizeof(Particle),
			reinterpret_cast<void*>(offsetof(Particle, velocity))
		);

	glBindVertexArray(0);
}

Mesh::~Mesh () {
	glDeleteVertexArrays(1, &handle_);
}
#include "particle_mesh.hh"
#include <glte/glte.hh>

#include <cassert>
#include <cstddef>

ParticleMesh::ParticleMesh ()
	: ParticleMesh (GL_STATIC_DRAW) {
}

ParticleMesh::ParticleMesh (GLenum vertex_hint)
	: ParticleMesh (new Buffer(vertex_hint)) {
}

ParticleMesh::ParticleMesh (Buffer* vertex_buffer)
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
		2, GL_FLOAT,
		GL_FALSE,
		sizeof(Vec2f),
		0
	);

	glBindVertexArray(0);
}

ParticleMesh::~ParticleMesh () {
	glDeleteVertexArrays(1, &handle_);
}
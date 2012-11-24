#include "mesh.hh"
#include "buffer.hh"
#include "shader.hh"

#include <cassert>
#include <cstddef>

namespace te {
	Mesh::Mesh ()
		: Mesh (GL_STATIC_DRAW, GL_STATIC_DRAW) {
	}

	/**
	 * \todo use ref-counting to avoid leaks
	 */
	Mesh::Mesh (GLenum vertex_hint, GLenum element_hint)
		: Mesh (new VertexBuffer(vertex_hint), new ElementBuffer(element_hint)) {
	}

	Mesh::Mesh (GLenum vertex_hint)
		: Mesh (new VertexBuffer(vertex_hint), 0) {
	}

	Mesh::Mesh (VertexBuffer* vertex_buffer, ElementBuffer* element_buffer)
		: vertex_buffer_ (vertex_buffer),
		  element_buffer_ (element_buffer) {
	
		assert(vertex_buffer_ != 0);

		// VertexArrays allow to store the vertex and element buffers ids
		glGenVertexArrays(1, &handle_);
		glBindVertexArray(handle_);
		
		// bind the vertex and element buffers (if any)
		glBindBuffer(VertexBuffer::Target, vertex_buffer_->handle());
		
		if (element_buffer_ != 0)
			glBindBuffer(ElementBuffer::Target, element_buffer_->handle());

		// setup attributes
		glVertexAttribPointer(
			Shader::ATTRIB_POSITION,
			3, GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position))
		);

		glVertexAttribPointer(
			Shader::ATTRIB_NORMAL,
			3, GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal))
		);

		glVertexAttribPointer(
			Shader::ATTRIB_COLOR,
			4, GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, color))
		);

		glVertexAttribPointer(
			Shader::ATTRIB_TEXCOORD,
			3, GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, texcoord))
		);

		glBindVertexArray(0);
	}

	Mesh::~Mesh () {
		glDeleteVertexArrays(1, &handle_);
	}
}
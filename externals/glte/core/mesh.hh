#pragma once

#include "common.hh"

namespace te {
	class VertexBuffer;
	class ElementBuffer;

	/**
	 * \todo maybe rename this class as MeshBuffer and create a MeshBuffer
	 * container named Mesh
	 */
	class Mesh {
	public:
		Mesh ();
		Mesh (GLenum vertex_hint, GLenum element_hint);
		Mesh (GLenum vertex_hint);
		Mesh (VertexBuffer* vertex_buffer, ElementBuffer* element_buffer);
		~Mesh ();

		// accessors

		GLuint handle () const { return handle_; }
		VertexBuffer* vertex_buffer () { return vertex_buffer_; }
		const VertexBuffer* vertex_buffer () const { return vertex_buffer_; }
		ElementBuffer* element_buffer () { return element_buffer_; }
		const ElementBuffer* element_buffer () const { return element_buffer_; }

	private:
		GLuint handle_;
		VertexBuffer* vertex_buffer_;
		ElementBuffer* element_buffer_;
	};
}
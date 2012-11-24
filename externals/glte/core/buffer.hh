#pragma once

#include "common.hh"
#include "vertex.hh"

#include <vector>
#include <array>

namespace te {
	template <GLenum GLTarget, typename ContentType>
	class Buffer {
	public:
		static const GLenum Target = GLTarget;

		// constructors and destructors

		/**
		 * @todo use a generic enum instead
		 */
		Buffer (GLenum hint);

		~Buffer ();

		/// @name Accessors
		/// @{

		GLuint handle () const { return handle_; }
		uint size () const { return size_; }

		/// @}

		// Methods

		/**
		 * @todo do something with glMapBufferRange and glMapBuffer
		 * http://www.opengl.org/wiki/Buffer_Object#Buffer_Object_Usage
		 */
		void data (const std::vector<ContentType>& data);

		template <std::size_t N>
		void data (const std::array<ContentType, N>& data);

		void reserve (uint size);

	private:
		GLuint handle_;
		GLenum hint_;
		uint size_;
	};

	class VertexBuffer : public Buffer<GL_ARRAY_BUFFER, Vertex> {
	public:
		VertexBuffer (GLenum hint)
			: Buffer (hint) {
		}
	};

	class ElementBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, uint> {
	public:
		ElementBuffer (GLenum hint)
			: Buffer (hint) {
		}
	};

	// ---

	template <GLenum GLTarget, typename ContentType> 
	Buffer<GLTarget, ContentType>::Buffer (GLenum hint)
		: hint_ (hint),
		  size_ (0) {
		glGenBuffers(1, &handle_);
	}

	template <GLenum GLTarget, typename ContentType> 
	Buffer<GLTarget, ContentType>::~Buffer () {
		glDeleteBuffers(1, &handle_);
	}

	template <GLenum GLTarget, typename ContentType> 
	void Buffer<GLTarget, ContentType>::data (const std::vector<ContentType>& data) {
		glBindBuffer(GLTarget, handle_);

		glBufferData(
			GLTarget,
			data.size() * sizeof(ContentType),
			data.data(),
			hint_
		);

		glBindBuffer(GLTarget, 0);

		size_ = data.size();
	}

	template <GLenum GLTarget, typename ContentType>
	template <std::size_t N>
	void Buffer<GLTarget, ContentType>::data (const std::array<ContentType, N>& data) {
		glBindBuffer(GLTarget, handle_);

		glBufferData(
			GLTarget,
			data.size() * sizeof(ContentType),
			data.data(),
			hint_
		);

		glBindBuffer(GLTarget, 0);

		size_ = data.size();
	}

	template <GLenum GLTarget, typename ContentType>
	void Buffer<GLTarget, ContentType>::reserve (uint size) {
		glBindBuffer(GLTarget, handle_);

		glBufferData(
			GLTarget,
			size * sizeof(ContentType),
			0,
			hint_
		);

		glBindBuffer(GLTarget, 0);

		size_ = size;
	}
}
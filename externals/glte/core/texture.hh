#pragma once

#include "common.hh"

#include <initializer_list>
#include <utility>

namespace te {
	/**
	 * @brief Wrapper for OpenGL texture objects
	 * @todo correct handling of mipmap levels
	 * @todo use an image class instead of plain data for uploading
	 */
	class Texture {
	public:
		Texture (GLenum type);

		~Texture ();

		/// @name Accessors
		/// @{

		GLenum type () const { return type_; }
		GLuint handle () const { return handle_; }

		/// @}

		void bind () const;
		void unbind () const;

		void parameter (const std::initializer_list<std::pair<GLenum, int>> list);

		/**
		 * @brief Set single channel data
		 * @param data Pixels data, in row-major mode for 2D textures
		 * @todo paremeterize with data formats
	 	 */
		void data (const float* data, const Vec2i size);
		void subdata (const float* data, const Vec2i offset, const Vec2i size);

		void data (const float* data, const Vec3i size);
		void subdata (const float* data, const Vec3i offset, const Vec3i size);

	private:
		GLenum type_;
		GLuint handle_;
	};

	// ---

	inline void Texture::bind () const {
		glBindTexture(type_, handle_);
	}

	inline void Texture::unbind () const {
		glBindTexture(type_, 0);
	}
}
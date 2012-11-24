#include "texture.hh"

namespace te {
	Texture::Texture (GLenum type)
		: type_ (type) {
		glGenTextures(1, &handle_);
		CHECK_GL_ERROR();
	}

	Texture::~Texture () {
		glDeleteTextures(1, &handle_);
	}

	void Texture::parameter (const std::initializer_list<std::pair<GLenum, int>> list) {
		glBindTexture(type_, handle_);
		
		for (auto it = list.begin(); it != list.end(); it++) {
			glTexParameteri(type_, it->first, it->second);
			CHECK_GL_ERROR();
		}

		glBindTexture(type_, 0);
	}

	// 2D

	void Texture::data (const float* data, const Vec2i size) {
		assert(type_ == GL_TEXTURE_2D); /// \todo or other types which can handle 2D images
		glBindTexture(type_, handle_);

		glTexImage2D(
			type_,
			0, // mipmap level
			GL_RED, // internal format
			size.x(), size.y(),
			0, // 0 to follow the OpenGL3 specifications (ex border size)
			GL_RED, // data format
			GL_FLOAT, // data values type
			data
		);

		CHECK_GL_ERROR();

		glBindTexture(type_, 0);
	}

	void Texture::subdata (const float* data, const Vec2i offset, const Vec2i size) {
		assert(type_ == GL_TEXTURE_2D); /// \todo or other types which can handle 2D images
		glBindTexture(type_, handle_);

		glTexSubImage2D(
			type_,
			0, // mipmap level
			offset.x(), offset.y(),
			size.x(), size.y(),
			GL_RED,
			GL_FLOAT,
			data
		);

		CHECK_GL_ERROR();

		glBindTexture(type_, 0);
	}

	// 3D

	void Texture::data (const float* data, const Vec3i size) {
		assert(type_ == GL_TEXTURE_3D); /// \todo or other types which can handle 3D images
		glBindTexture(type_, handle_);

		glTexImage3D(
			type_,
			0, // mipmap level
			GL_RED, // internal format
			size.x(), size.y(), size.z(),
			0, // 0 to follow the OpenGL3 specifications (ex border size)
			GL_RED, // data format
			GL_FLOAT, // data values type
			data
		);

		CHECK_GL_ERROR();

		glBindTexture(type_, 0);
	}

	void Texture::subdata (const float* data, const Vec3i offset, const Vec3i size) {
		assert(type_ == GL_TEXTURE_3D); /// \todo or other types which can handle 2D images
		glBindTexture(type_, handle_);

		glTexSubImage3D(
			type_,
			0, // mipmap level
			offset.x(), offset.y(), offset.z(),
			size.x(), size.y(), size.z(),
			GL_RED,
			GL_FLOAT,
			data
		);

		CHECK_GL_ERROR();

		glBindTexture(type_, 0);
	}	
}
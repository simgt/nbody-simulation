#pragma once

// INCLUDES

#include <Eigen/Dense>
#include <Eigen/Geometry>

// TYPES

typedef unsigned int uint;
typedef unsigned char uchar;

typedef Eigen::Vector2f Vec2f;
typedef Eigen::Vector3f Vec3f;
typedef Eigen::Vector4f Vec4f;

typedef Eigen::Vector2i Vec2i;
typedef Eigen::Vector3i Vec3i;

typedef Eigen::Matrix4f Mat4f;

using Eigen::Affine3f;
using Eigen::Projective3f;

// DEFINES

#ifndef M_PI
#define M_PI 3.141592653589793
#endif


// DEBUG

#include <cassert>

#ifdef NDEBUG
#define outlog(x)
#else
#include <iostream>
#define outlog(x) \
	std::cout << #x << ": " << x << std::endl;
#endif

// MATHS

namespace te {
	inline const Projective3f perspective (float fov, float aspect, float near, float far) {
	    Projective3f p;

	    float focal = 1.0f / std::tan(fov / 2.0f); // 1 / tan(fov_in_rad / 2)
	    
	    p.matrix() << focal / aspect, 0,    0,                           0,
	    			  0,             focal, 0,                           0,
	    			  0,             0,     (near + far) / (far - near), -2.0f * near * far / (far - near),
	    			  0,             0,     1,                           0;
	    /// @todo check why the 1 on the last row is not a -1

	    return p;
	}
}

// CORE

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

#include <iostream>
#ifndef NDEBUG
#define CHECK_GL_ERROR() {										\
	int error = glGetError();									\
	if (error != GL_NO_ERROR) {									\
		std::cout << __FILE__ << ' ' << __LINE__ << ": ";		\
		switch (error) {										\
			case GL_INVALID_ENUM:								\
				std::cerr << "GL_INVALID_ENUM";					\
				break;											\
			case GL_INVALID_VALUE:								\
				std::cerr << "GL_INVALID_VALUE";				\
				break;											\
			case GL_INVALID_OPERATION:							\
				std::cerr << "GL_INVALID_OPERATION";			\
				break;											\
			case GL_INVALID_FRAMEBUFFER_OPERATION:				\
				std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";\
				break;											\
			case GL_OUT_OF_MEMORY:								\
				std::cerr << "GL_OUT_OF_MEMORY";				\
				break;											\
		}														\
		std::cerr << std::endl;									\
		exit(1);												\
	}															\
}
#else
#define CHECK_GL_ERROR()
#endif

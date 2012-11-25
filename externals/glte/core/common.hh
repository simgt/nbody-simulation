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

// DEFINES

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

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

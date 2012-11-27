#include "shader.hh"

extern "C" {
#include <glsw/glsw.h>
}

#include <map>
#include <fstream>

namespace {
	const char* attributes_names[] = {
		"position",
		"normal",
		"texcoord",
		"color"
	};
}

namespace te {
	Shader::Shader (GLuint handle)
		: handle_ (handle) {

		glUseProgram(handle);

		// uniforms
		uniforms_.mv_matrix = glGetUniformLocation(handle, "mv_matrix");
		uniforms_.p_matrix = glGetUniformLocation(handle, "p_matrix");

		// set attributes flags
		attributes_ = 0;
		for (uint i = 0; i < NUM_ATTRIBS; i++) {
			if (glGetAttribLocation(handle, attributes_names[i]) != -1)
				attributes_ |= 1 << i;
		}

		glUseProgram(0);

		CHECK_GL_ERROR();
	}

	Shader::~Shader () {
		//@todo
	}
}

namespace {
	std::map<std::string, GLuint> shaders;
	std::map<std::pair<std::string, std::string>, te::Shader*> programs;

	GLuint load_shader(GLenum type, const std::string& key) {
		// if the shader has already been compiled, return it
		if (shaders.count(key))
			return shaders[key];

		const char* source = glswGetShader(key.c_str());
	
		if (!source) {
			std::cerr << glswGetError() << std::endl;
			return 0;
		}

		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, &source, 0);
		glCompileShader(handle);

		GLint success;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLint len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
			char info[len];
			glGetShaderInfoLog(handle, len, 0, info);
			CHECK_GL_ERROR();
			std::cerr << "Failed to compile " << key << ":"
					  << info << std::endl;

			glDeleteShader(handle);
			return 0; // \todo throw an exception
		}

		shaders[key] = handle;

		return handle;
	}
}

namespace te {
	Shader* Shader::load (const std::string& vertex_key, const std::string& fragment_key) {
		/// \todo Use Boost.Flyweight
		// if the program has already been compiled, return it
		auto pair = std::make_pair(vertex_key, fragment_key);
		if (programs.count(pair))
			return programs[pair];

		// program building
		GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_key);
		GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_key);

		GLuint handle = glCreateProgram();

		// bind generic attributes locations
		for (uint i = 0; i < NUM_ATTRIBS; i++)
			glBindAttribLocation(handle, i, attributes_names[i]);

		CHECK_GL_ERROR();

		// attach shaders
		glAttachShader(handle, vertex_shader);
		glAttachShader(handle, fragment_shader);

		glLinkProgram(handle);

		CHECK_GL_ERROR();

		GLint success;
		glGetProgramiv(handle, GL_LINK_STATUS, &success);
		if (!success) {
			GLint len;
			glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
			char info[len];
			glGetProgramInfoLog(handle, len, NULL, info);
			
			std::cerr << "Failed to link shader program:"
					  << info << std::endl;

			glDeleteProgram(handle);
			return 0;
		}

		Shader* program = new Shader(handle);
		programs[pair] = program;

		return program;
	};
}
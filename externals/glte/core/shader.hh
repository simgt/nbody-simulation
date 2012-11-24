#pragma once

#include "common.hh"

#include <string>

namespace te {
	class Shader {
	public:
		enum Attribute {
			ATTRIB_POSITION = 0,
			ATTRIB_NORMAL,
			ATTRIB_TEXCOORD,
			ATTRIB_COLOR,
			NUM_ATTRIBS
		};

		Shader (GLuint handle);

		/**
		 * \todo Delete OpenGL resources
		 */
		~Shader ();

		/// \name Accessors
		///@{

		/**
		 * \return OpenGL name associated to the shader program
		 */
		GLuint handle () const { return handle_; }

		/**
		 * \return true if the shader uses the attribute, false otherwise
		 */
		bool has_attribute (const Attribute attr) const {
			return attributes_ & (1 << attr); ///
		}

		///@}

		static Shader* load (const std::string& vertex_file, const std::string& fragment_file);

	private:
		GLuint handle_;

		struct {
			GLuint mv_matrix, p_matrix;
		} uniforms_;

		uint attributes_; // bitwise flags

		friend class Mesh;
		friend class Driver;
	};
}
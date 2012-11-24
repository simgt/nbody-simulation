#include "driver.hh"
#include "buffer.hh"

namespace te {
	/// @todo put mode and shader in a fucking material class
	void Driver::draw (const Mesh& mesh, const Shader& shader, GLenum mode) const {
		// bind the shader
		glUseProgram(shader.handle());

		/* --------------------- *
		 * setup common uniforms *
		 * --------------------- */

		// projection matrix
		glUniformMatrix4fv(
			shader.uniforms_.p_matrix,
			1, // count
			false, // transpose
			projection_.data()
		);

		CHECK_GL_ERROR();

		// modelview matrix
		/// \todo multiply by the camera matrix
		glUniformMatrix4fv(
			shader.uniforms_.mv_matrix,
			1, // count
			false, // transpose
			modelview_.data()
		);

		CHECK_GL_ERROR();

		/* ------------ *
		 * draw objects *
		 * ------------ */

		// bind the VAO associated to the mesh
		glBindVertexArray(mesh.handle());
		
		CHECK_GL_ERROR();

		// enable/disable attributes depending on the shader
		for (uint i = 0; i < Shader::NUM_ATTRIBS; i++) {
			if (shader.has_attribute(Shader::Attribute(i)))
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
		}

		CHECK_GL_ERROR();

		// draw mesh's elements
		glDrawElements(
			mode,
			mesh.element_buffer()->size(),
			GL_UNSIGNED_INT,
			0
		);

		CHECK_GL_ERROR();

		// clear states
		glBindVertexArray(0);
		glUseProgram(0);
	}
}
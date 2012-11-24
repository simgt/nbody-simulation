#include <glte/glte.hh>

#include "particle_mesh.hh"

void draw (
		te::Context& context,
		te::Shader& shader,
		ParticleMesh& mesh
	) {
	context.clear();
	glUseProgram(shader.handle());

    // bind the VAO associated to the mesh
    glBindVertexArray(mesh.handle());
    
    CHECK_GL_ERROR();

    // enable/disable attributes depending on the shader
    for (uint i = 0; i < te::Shader::NUM_ATTRIBS; i++) {
        if (shader.has_attribute(te::Shader::Attribute(i)))
            glEnableVertexAttribArray(i);
        else
            glDisableVertexAttribArray(i);
    }

    CHECK_GL_ERROR();

    // draw mesh's elements
    glPointSize(2);
    glDrawArrays(
        GL_POINTS,
        0,
        mesh.vertex_buffer()->size()
    );

    CHECK_GL_ERROR();

    // clear states
    glBindVertexArray(0);
    glUseProgram(0);

    context.swap();
}
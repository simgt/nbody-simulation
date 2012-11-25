#include "common.hh"
#include "mesh.hh"

#include <glte/glte.hh>

void draw (
		te::Context& context,
		te::Shader& shader,
		Mesh& mesh
	) {
	context.clear();
	glUseProgram(shader.handle());

    // retrieve window size
    GLuint uniform_viewport_size = glGetUniformLocation(shader.handle(), "viewport_size");
    int width, height;
    glfwGetWindowSize(&width, &height);
    Vec2f viewport_size(width, height);
    glUniform2fv(uniform_viewport_size, 1, viewport_size.data());

    GLuint uniform_point_size = glGetUniformLocation(shader.handle(), "point_size");
    glUniform1f(uniform_point_size, PARTICLES_SIZE);

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
    glPointSize(20);
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
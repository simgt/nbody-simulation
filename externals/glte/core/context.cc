#include "context.hh"
#include <string>
#include <sstream>

extern "C" {
#include <glsw/glsw.h>
}

namespace te {
	Context::Context (
			int width, int height,
			bool fullscreen,
			const char* title
		) {

		// ------------------
		// Resources managers
		// ------------------

		glswInit();
		glswSetPath("runtime/", ".glsl");
		glswAddDirectiveToken("", "#version 150");

		// ----
		// GLFW
		// ----

		glfwInit();

		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
		glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

		bool ok = glfwOpenWindow(
						width, height,
						8, 8, 8, 8, // r, g, b, a
						24, 8, // depth, stencil
						fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW
					);

		if (!ok) {
			std::cerr << "ERROR: Unable to create the OpenGL context and associated window" << std::endl;
			glfwTerminate();
			exit(1);
		}

		glfwSetWindowTitle(title);
		glfwEnable(GLFW_AUTO_POLL_EVENTS);
		//glfwSetWindowSizeCallback(window_resize);
		
		// http://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
		// start the clock at ~4 billions seconds in order to have the same
		// precision for the next ~136 years
		glfwSetTime(2 << 31);

		// opengl init

		//check_glerror();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // default
		glFrontFace(GL_CCW); // default

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f); // default

	    // alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearDepth(1.0f);
		glClearColor(0, 0, 0, 1);
	}

	Context::~Context () {
		// ------------------
		// Resources managers
		// ------------------
		
		glswShutdown();

		// ----
		// GLFW
		// ----

		if (glfwGetWindowParam(GLFW_OPENED))
			glfwCloseWindow();
		
		glfwTerminate();
	}

	float Context::time () const {
		static double last = 0;
		double current = glfwGetTime();
		float elapsed = current - last;
		last = current;

		return elapsed;
	}
}

#pragma once

#include "common.hh"
#include <cstdlib>

namespace te {
	class Context {
	public:
		Context (
				int width, int height,
				bool fullscreen = false,
				const char* title = "te"
			);

		~Context ();

		/// @name Accessors
		/// @{

		bool exist ();

		void clear_color (const Vec4f c);

		/// @}

		void clear ();

		void swap ();

		/**
		 * @brief elapsed time in seconds since the last call
		 */
		float time () const;

		//template <typename T>
		//void register_observer (T* obj) {
		//	void (T::*pmethod) (int x, int y);
		//	pmethod = &T::mouse_position_hook;
		//}

	};

	// ---

	inline bool Context::exist () {
		return !glfwGetKey(GLFW_KEY_ESC)
			&& glfwGetWindowParam(GLFW_OPENED);
	}

	inline void Context::clear_color (const Vec4f c) {
		glClearColor(c[0], c[1], c[2], c[3]);
	}

	inline void Context::clear () {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void Context::swap () {
		glfwSwapBuffers();

		//if (elapsed < MAX_REFRESH)
		//	glfwSleep(MAX_REFRESH - elapsed);
	}
}
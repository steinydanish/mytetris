#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "glew.h"
#include "glfw3.h"

class Application
{
	private :
		GLFWwindow* window;
		const char* glsl_version;
		void processInput(GLFWwindow* win);
	public :
        void mainLoop();
		int showWindow();
		void cleanup();
};

#endif

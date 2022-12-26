#include <iostream>
#include <unistd.h> // for usleep

#include "OpenGL.hpp"

void main_process () {
	std::cout << "[MODE START]" << std::endl;
	OpenGL GL;

	std::cout << C_BOLD << "[MAIN] : Loop starts" << C_RESET << std::endl;
	while (GL.OK())
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(GL.GetWindow()->Id());
    	glfwPollEvents(); 
	}
}


int main() {
	try
	{
		main_process();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}
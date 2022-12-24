#ifndef OPENGL_HPP
# define OPENGL_HPP

/* CPP */
# include <iostream>
# include <exception>
# include <vector>

/* OpenGL */
# include <glad.h>
# include <GLFW/glfw3.h>

/* Custom */
#include "CharColor.hpp"
#include "Window.hpp"

void FrameBufferSizeCB(GLFWwindow * window, int width, int height);
void KeyboardCB(GLFWwindow * window, int key, int scancode, int action, int mods);
void MouseMoveCB(GLFWwindow * w, double x, double y);
void MouseClickCB(GLFWwindow * w, int button, int action, int mods);

class OpenGL
{
private:
	Window * _window;
public:
	OpenGL( void );
	~OpenGL();

	bool OK() const { return (glfwWindowShouldClose(_window->Id()) == false);}

	/*Getter*/
	Window * GetWindow() const { return _window; }
private :
	void _Terminate();
	void _InitCallbackFunctions();
};

void OpenGL::_Terminate()
{
	delete _window;
	glfwTerminate();
}

OpenGL::OpenGL( void )
: _window(NULL)
{
	// glfw 초기화
	if (!glfwInit()) {
		throw std::runtime_error("[OpenGL] : fail to init glfw");
	}
	std::cout << C_YELLOW << "HEELO OPENGL" << C_RESET <<std::endl;
	try {
	 	_window = new Window(800, 600, "OpenGL");
		glfwMakeContextCurrent(_window->Id()); // Use this Window
	} catch (const std::exception &e) {
		_Terminate();
		throw std::runtime_error(e.what());
	}

	// glfwMakeContextCurrent()를 호출하여 opengl context를 지정해주고 glad를 초기화 해야 한다.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		_Terminate();
		throw std::runtime_error("[OpenGL] : fail to init GLAD");
	}
	
	_InitCallbackFunctions();
}

OpenGL::~OpenGL()
{
	_Terminate();
	std::cout << C_YELLOW << "GOOD BY OPENGL" << C_RESET << std::endl;
}

void OpenGL::_InitCallbackFunctions()
{
	glfwSetKeyCallback(_window->Id(), KeyboardCB);
	glfwSetFramebufferSizeCallback(_window->Id(), FrameBufferSizeCB);
}

void FrameBufferSizeCB(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void KeyboardCB(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void MouseMoveCB(GLFWwindow * w, double x, double y)
{

}

void MouseClickCB(GLFWwindow * w, int button, int action, int mods)
{

}

#endif
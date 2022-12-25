#ifndef OPENGL_HPP
# define OPENGL_HPP

/* CPP */
# include <iostream>
# include <exception>

/* OpenGL */
# include <glad.h>
# include <GLFW/glfw3.h>
//# ifndef STB_IMAGE_IMPLEMENTATION
//#  define STB_IMAGE_IMPLEMENTATION
//# endif
//# include <stb_image.h>

/* Custom */
#include "CharColor.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

void FrameBufferSizeCB(GLFWwindow * window, int width, int height);
void KeyboardCB(GLFWwindow * window, int key, int scancode, int action, int mods);
void MouseMoveCB(GLFWwindow * w, double x, double y);
void MouseClickCB(GLFWwindow * w, int button, int action, int mods);

class OpenGL
{
private:
	static OpenGL * opengl_;
	Window * window_;
	Shader * shader_;
	Camera * camera_;
	OpenGL( void );
public:
	static OpenGL * getInstance();
	~OpenGL();

	bool OK() const { return (glfwWindowShouldClose(window_->Id()) == false);}

	/*Getter*/
	Window * GetWindow() const { return window_; }
	GLuint GetShaderID() const { return shader_->Id(); }
	const Camera * GetCamera() const { return camera_; }
	Camera * GetCamera() { return camera_; }
	/*Setter*/
	void SetShader(const std::string & vert, const std::string & frag);
	void SetCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
private :
	void _Terminate();
	void _InitCallbackFunctions();
};

OpenGL * OpenGL::opengl_ = NULL;

OpenGL * OpenGL::getInstance()
{
	if (opengl_ == NULL)
		opengl_ = new OpenGL();
	return opengl_;
}

OpenGL::OpenGL( void )
: window_(NULL), shader_(NULL), camera_(NULL)
{
	// glfw 초기화
	if (!glfwInit()) {
		throw std::runtime_error("[OpenGL] : fail to init glfw");
	}
	std::cout << C_YELLOW << "HEELO OPENGL" << C_RESET <<std::endl;
	try {
	 	window_ = new Window(800, 600, "OpenGL");
		glfwMakeContextCurrent(window_->Id()); // Use this Window
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

void OpenGL::_Terminate()
{
	delete window_;
	delete shader_;
	delete camera_;
	glfwTerminate();
}

void OpenGL::_InitCallbackFunctions()
{
	glfwSetKeyCallback(window_->Id(), KeyboardCB);
	glfwSetFramebufferSizeCallback(window_->Id(), FrameBufferSizeCB);
	glfwSetCursorPosCallback(window_->Id(), MouseMoveCB);
	glfwSetMouseButtonCallback(window_->Id(), MouseClickCB);
}

void OpenGL::SetShader(const std::string & vert, const std::string & frag)
{
	shader_ = new Shader(vert, frag);
	if (shader_ == NULL)
		throw std::runtime_error("[OpenGL] : fail to set shader");
}

void OpenGL::SetCamera(glm::vec3 eye = glm::vec3(10,0,0), 
						glm::vec3 center = glm::vec3(0,0,0),
						glm::vec3 up = glm::vec3(0,0,1))
{
	camera_ = new Camera(eye, center, up);
	if (shader_ == NULL)
		throw std::runtime_error("[OpenGL] : fail to set camera");
}


/**************/
/*  CallBack  */
/**************/
void FrameBufferSizeCB(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void KeyboardCB(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		Camera * cam = OpenGL::getInstance()->GetCamera();
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		} else if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
			cam->go_foward();
		} else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
			cam->go_backward();
		} else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
			cam->go_left();
		} else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
			cam->go_right();
		} else if (key == GLFW_KEY_SPACE) {
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				cam->go_down();
			else
				cam->go_up();
		}
	}
}

void MouseMoveCB(GLFWwindow * w, double xposIn, double yposIn)
{
	Window * window = OpenGL::getInstance()->GetWindow();
	if (window->IsMouseClicked() == false) {
		return ;
	}

	float lastx = window->GetMouseX();
	float lasty = window->GetMouseY();
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	std::cout << "(" << xpos << "," << ypos << ")" << std::endl;
	if (lastx < 0 || lasty < 0)
	{
		window->SetMouseX(xpos);
		window->SetMouseY(ypos);
		return ;
	}

	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos; // reversed since y-coordinates go from bottom to top
	window->SetMouseX(xpos);
	window->SetMouseY(ypos);

	float sensitivity = 0.42f; // change this value to your liking

	Camera * cam = OpenGL::getInstance()->GetCamera();
	cam->rotate(glm::radians(sensitivity), glm::normalize(glm::vec3(-yoffset, xoffset, 0.0f)));
}

void MouseClickCB(GLFWwindow * w, int button, int action, int mods)
{
	Window * window = OpenGL::getInstance()->GetWindow();
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		window->SetMouseClicked(true);
	else
		window->SetMouseClicked(false);
}

#endif
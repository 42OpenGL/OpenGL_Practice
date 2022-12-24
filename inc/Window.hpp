#ifndef WINDOW_HPP
# define WINDOW_HPP
# include <iostream>

# include <glad.h>
# include <GLFW/glfw3.h>

class Window
{
private:
	int _width;
	int _height;
	std::string _name;
	GLFWwindow * _window_ptr;
public :
public:
	Window(int, int, const std::string &);
	~Window();

	/* Getter */
	GLFWwindow * Id() const { return _window_ptr; }

	/* Setter */
	/* Method */
private :
	/* Private Method */
};

Window::Window(int w, int h, const std::string &n)
: _width(w), _height(h), _name(n), _window_ptr(nullptr)
{
	glfwWindowHint(GLFW_SAMPLES, 4); // 안티엘리어싱 x4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 최대버전: 그냥 glfw 버전
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 최소버전: 그냥 glfw 버전
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 프로파일 버전: 코어

	_window_ptr = glfwCreateWindow(_width, _height, _name.c_str(), 0, 0);
	if (_window_ptr == NULL) {
		throw std::runtime_error("[Window] : fail to create window");
	}
}

Window::~Window()
{
	glfwDestroyWindow(_window_ptr);
	std::cout << C_YELLOW << "Closing Window" << C_RESET << std::endl;
}
#endif
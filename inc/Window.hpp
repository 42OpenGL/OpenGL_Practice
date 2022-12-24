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
	bool is_mouse_clicked_;
	float mouse_x_, mouse_y_;
public :
public:
	Window(int, int, const std::string &);
	~Window();

	/* Getter */
	GLFWwindow * Id() const { return _window_ptr; }
	bool IsMouseClicked() const { return is_mouse_clicked_; }
	float GetMouseX() const { return mouse_x_; }
	float GetMouseY() const { return mouse_y_; }

	/* Setter */
	void SetMouseClicked(bool click) { is_mouse_clicked_ = click; }
	void SetMouseX(float p) { mouse_x_ = p; }
	void SetMouseY(float p) { mouse_y_ = p; }

	/* Method */
private :
	/* Private Method */
};

Window::Window(int w, int h, const std::string &n)
: _width(w), _height(h), _name(n), _window_ptr(nullptr)
, is_mouse_clicked_(false), mouse_x_(-1), mouse_y_(-1)
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

// GLFWwindow 관련 leak문제가 있다. 아직 해결 하는 법 모르겠음.
Window::~Window()
{
	glfwDestroyWindow(_window_ptr);
	std::cout << C_YELLOW << "Closing Window" << C_RESET << std::endl;
}
#endif
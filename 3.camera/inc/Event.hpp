//#ifndef EVENT_HPP
//# define EVENT_HPP
//# include "Window.hpp"

//class Event
//{
//public :
//	void KeyboardCB(GLFWwindow * w, int key, int scancode, int action, int mods);
//	void MouseMoveCB(GLFWwindow * w, double x, double y);
//	void MouseClickCB(GLFWwindow * w, int button, int action, int mods);
//public :
//	typedef void (Event::*keyboardCBptr)(GLFWwindow *, int, int, int, int);
//	typedef void (Event::*mousemoveCBptr)(GLFWwindow *, double, double);
//	typedef void (Event::*mousclickCBptr)(GLFWwindow *, int, int, int);
//	Event();
//	~Event();

//	/* GETTER */
//	keyboardCBptr GetKeyboardCB() { return &KeyboardCB; }
//	mousemoveCBptr GetMouseMoveCB() { return &MouseMoveCB; }
//	mousclickCBptr GetMouseClickCB() { return &MouseClickCB; }
//};

//Event::Event()
//{

//}

//void Event::KeyboardCB(GLFWwindow * window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}

//void Event::MouseMoveCB(GLFWwindow * w, double x, double y)
//{

//}

//void Event::MouseClickCB(GLFWwindow * w, int button, int action, int mods)
//{

//}

//#endif
#ifndef WINDOW_H
#define WINDOW_H

// Debian ImGui needs glew
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const unsigned int INITIAL_WINDOW_WIDTH = 1200;
const unsigned int INITIAL_WINDOW_HEIGHT = 900;
const float SPHERE_DETAILS = 64.0f;

class Window
{
public:
	Window();
	~Window();
	static int CheckGLError(const char * str = "");
	void Clear();
	void Swap();

	GLFWwindow * GetWindow() { return window; }
	unsigned int GetWindowHeight() { glfwGetWindowSize(window, &width, &height); return height; }

private:
	void InitOpenGL();

	GLFWwindow * window;

	int width;
	int height;
};

#endif // WINDOW_H

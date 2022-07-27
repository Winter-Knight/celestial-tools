#include <cstring>

#include "input.h"

using namespace celestial;

void InputHandler::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_REPEAT)
		return;

	keyStates[key] = action;

	if (action == GLFW_RELEASE)
		return;

	lastKey = key;
	buttonMask = mods;

	switch(key) {
		case GLFW_KEY_ESCAPE:
			quit = 1;
			break;
		case GLFW_KEY_SPACE:
			paused = !paused;
			break;
		case GLFW_KEY_F8:
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case GLFW_KEY_F11:
			int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
			if (maximized)
				glfwRestoreWindow(window);
			else
				glfwMaximizeWindow(window);
			break;
	}
}

void InputHandler::mouseMotionCallback(GLFWwindow * window, double x, double y)
{
	xpos = x;
	ypos = y;

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		xrel = xpos - centerPosX;
		yrel = ypos - centerPosY;
	}
}

void InputHandler::mouseButtonCallback(GLFWwindow * window, int b, int action, int mods)
{
	if (action == GLFW_PRESS) {
		button = b;
		buttonMask |= button;
	}
	else
		buttonMask = 0;
}

void InputHandler::windowSizeCallback(GLFWwindow * window, int width, int height)
{
	windowEvent = 1;
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
}

void InputHandler::Init(GLFWwindow * w)
{
	window = w;
	
	glfwSetKeyCallback(window, InputHandler::keyCallback);
	glfwSetCursorPosCallback(window, InputHandler::mouseMotionCallback);
	glfwSetMouseButtonCallback(window, InputHandler::mouseButtonCallback);
	glfwSetWindowSizeCallback(window, InputHandler::windowSizeCallback);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	centerPosX = width / 2;
	centerPosY = height / 2;

	memset(keyStates, 0, GLFW_KEY_LAST + 1);
}

void InputHandler::Reset()
{
	static int firstTime = true;
	
	if (!firstTime && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		glfwSetCursorPos(window, centerPosX, centerPosY);
	}
	firstTime = false;

	lastKey = xrel = yrel = buttonMask = windowEvent = 0;
	button = xpos = ypos = 0;
}

bool InputHandler::PollEvent(void * x)
{
	static bool sw = false;
	glfwPollEvents();
	sw = !sw;
	return sw;
}

unsigned char * InputHandler::GetKeyStates(void * x)
{
	return keyStates;
}

int InputHandler::HandleEvents(void * event, Window * window)
{
	static int firstTime = true;
	
	if (firstTime) {
		Init(window->GetWindow());
		firstTime = false;
	}

	if (glfwWindowShouldClose(window->GetWindow()))
		quit = 1;

	return quit;
}

static const float speed = 0.0010f; // 1 units / second
static const float mouseSpeed = 0.005f;

int InputHandler::lastKey = 0;
int InputHandler::xrel = 0;
int InputHandler::yrel = 0;
int InputHandler::buttonMask = 0;
int InputHandler::windowEvent = 0;
int InputHandler::windowWidth = INITIAL_WINDOW_WIDTH;
int InputHandler::windowHeight = INITIAL_WINDOW_HEIGHT;
int InputHandler::button = 0;
int InputHandler::xpos = 0;
int InputHandler::ypos = 0;

int InputHandler::quit = 0;
int InputHandler::paused = 0;

int InputHandler::centerPosX = 0;
int InputHandler::centerPosY = 0;

unsigned char InputHandler::keyStates[GLFW_KEY_LAST + 1];
GLFWwindow * InputHandler::window = NULL;

#ifndef INPUT_H
#define INPUT_H

#include "window.h"
#include "camera.h"

class InputHandler {
public:
	
	static void Init(GLFWwindow * window);
	static void Reset();
//	int HandleEvents(Window * window);
	static int HandleEvents(void * event, Window * window);
	static void Keyboard();
	static void MouseMove(Window * window);
	static void MouseButton();
	static void WindowEvent(Window * window);
	static bool PollEvent(void * x);

	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void mouseMotionCallback(GLFWwindow * window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow * window, int b, int action, int mods);
	static void windowSizeCallback(GLFWwindow * window, int width, int height);


	static unsigned char keyStates[GLFW_KEY_LAST + 1];
	static unsigned char * GetKeyStates(void *);

	static int lastKey;
	static int xrel, yrel, buttonMask;
	static int windowEvent, windowWidth, windowHeight;
	static int button, xpos, ypos;
	
	static int quit;
	static int paused;
	
	static int centerPosX;
	static int centerPosY;

	static GLFWwindow * window;
};

#endif // INPUT_H

#ifndef WINDOW_H
#define WINDOW_H

#ifndef _WIN32
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
#endif // _WIN32

#include <SDL2/SDL.h>

const unsigned int INITIAL_WINDOW_WIDTH = 1200;
const unsigned int INITIAL_WINDOW_HEIGHT = 900;
const float SPHERE_DETAILS = 64.0f;

class Window
{
public:
	Window();
	~Window();
	static int CheckSDLError(const char * str = "");
	static int CheckGLError(const char * str = "");
	void Clear();
	void Swap();

	SDL_Window * GetWindow() { return window; }
	unsigned int GetWindowHeight() { SDL_GetWindowSize(window, &width, &height); return height; }

private:
	void InitOpenGL();

	SDL_Window * window;
	SDL_GLContext context;

	int width;
	int height;
};

#endif // WINDOW_H

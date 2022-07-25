#ifndef WINDOW_H
#define WINDOW_H

// Debian ImGui needs glew
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "../texture.h"

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
	SDL_GLContext * GetContext() { return &context; }
	unsigned int GetWindowHeight() { SDL_GetWindowSize(window, &width, &height); return height; }
	void SetIcon(const char * filename);

private:
	void InitOpenGL();

	SDL_Window * window;
	SDL_GLContext context;

	int width;
	int height;
};

#endif // WINDOW_H

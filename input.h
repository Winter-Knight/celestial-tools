#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include "window.h"

class InputHandler {
public:
	
	void Reset();
	int HandleEvents(Window * window);
	int HandleEvents(SDL_Event * event, Window * window);
	void Keyboard(SDL_Event * event);
	void MouseMove(SDL_Event * event, Window * window);
	void MouseButton(SDL_Event * event);
	void WindowEvent(SDL_Event * event, Window * window);

	int lastKey;
	int xrel, yrel, buttonMask;
	int windowEvent, windowWidth, windowHeight;
	int button, xpos, ypos;
	
	int quit = 0;
	int paused = 0;

};

#endif // INPUT_H

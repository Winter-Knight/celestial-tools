#include "input.h"

int InputHandler::HandleEvents(Window * window)
{
	SDL_Event event;
	lastKey = xrel = yrel = buttonMask = windowEvent = 0;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			quit = 1;
		if (event.type == SDL_KEYDOWN)
			Keyboard(&event);
		if (event.type == SDL_MOUSEMOTION)
			MouseMove(&event, window);
		if (event.type == SDL_WINDOWEVENT)
			WindowEvent(&event, window);
	}
	return quit;
}

void InputHandler::Keyboard(SDL_Event * event)
{
	lastKey = event->key.keysym.sym;
	if (lastKey == SDLK_ESCAPE)
		quit = 1;
	if (lastKey == SDLK_SPACE)
		paused = !paused;
}

void InputHandler::MouseMove(SDL_Event * event, Window * window)
{
	if (event->motion.windowID == SDL_GetWindowID(window->GetWindow())) {
		xrel += event->motion.xrel;
		yrel += event->motion.yrel;
		buttonMask = event->motion.state;
	}
	else
		SDL_ClearError();
}

void InputHandler::WindowEvent(SDL_Event * event, Window * window)
{
	if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		windowEvent = 1;
		windowWidth = event->window.data1;
		windowHeight = event->window.data2;
		glViewport(0, 0, windowWidth, windowHeight);
	}
}

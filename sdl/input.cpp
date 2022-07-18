#include "input.h"

using namespace celestial;

void InputHandler::Reset()
{
	lastKey = xrel = yrel = buttonMask = windowEvent = 0;
	button = xpos = ypos = 0;
}	

int InputHandler::HandleEvents(Window * window)
{
	SDL_Event event;
	
	Reset();
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			quit = 1;
		if (event.type == SDL_KEYDOWN)
			Keyboard(&event);
		if (event.type == SDL_MOUSEMOTION)
			MouseMove(&event, window);
		if (event.type == SDL_WINDOWEVENT)
			WindowEvent(&event, window);
		if (event.type == SDL_MOUSEBUTTONDOWN)
			MouseButton(&event);
	}
	return quit;
}

int InputHandler::HandleEvents(SDL_Event * event, Window * window)
{
	if (event->type == SDL_QUIT)
		quit = 1;
	if (event->type == SDL_KEYDOWN)
		Keyboard(event);
	if (event->type == SDL_MOUSEMOTION)
		MouseMove(event, window);
	if (event->type == SDL_WINDOWEVENT)
		WindowEvent(event, window);
	if (event->type == SDL_MOUSEBUTTONDOWN)
		MouseButton(event);

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
	if (event->motion.windowID == SDL_GetWindowID(window->GetWindow()) && SDL_GetRelativeMouseMode()) {
		xrel += event->motion.xrel;
		yrel += event->motion.yrel;
		buttonMask = event->motion.state;
	}
	else
		SDL_ClearError();
}

void InputHandler::MouseButton(SDL_Event * event)
{
	button = event->button.button;
	xpos = event->button.x;
	ypos = windowHeight - event->button.y - 1;
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

static const float speed = 0.0010f; // 1 units / second
static const float mouseSpeed = 0.005f;

void InputHandler::UpdateCamera(Camera * camera)
{
	glm::vec3 dir = glm::normalize(camera->dir);
	glm::vec3 up = glm::normalize(camera->up);
	glm::vec3 right = glm::cross(dir, camera->up);
	glm::vec3 pos = camera->pos;

	// Mouse

	int axrel = abs(xrel);
	int ayrel = abs(yrel);

	if (!buttonMask) {
	// Yaw (left and right)
		dir = glm::normalize((dir * float(100 - axrel)) + (right * float(xrel)));
	}
	else if (buttonMask == SDL_BUTTON_RMASK) {
	// Roll (like rolling your head)
		up = glm::normalize((up * float(100 - axrel)) + (right * float(xrel)));
	}
	right = glm::cross(dir, up);

	// Pitch (up and down)
	dir = glm::normalize((dir * float(100 - ayrel)) + (up * float(yrel)));
	up = glm::cross(right, dir);


	// Keyboard
	
	const unsigned char * state = SDL_GetKeyboardState(NULL);

	float deltaTime = 16.0f;
	if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		deltaTime *= 60;
	if (state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT])
		deltaTime /= 60;

	if (state[SDL_SCANCODE_W]) {
			pos += dir * (deltaTime * speed);
	}
	if (state[SDL_SCANCODE_D]) {
			pos += right * (deltaTime * speed);
	}
	if (state[SDL_SCANCODE_S]) {
			pos -= dir * (deltaTime * speed);
	}
	if (state[SDL_SCANCODE_A]) {
			pos -= right * (deltaTime * speed);
	}
	if (state[SDL_SCANCODE_E]) {
			pos += up * (deltaTime * speed);
	}
	if (state[SDL_SCANCODE_Q]) {
			pos -= up * (deltaTime * speed);
	}

	camera->pos = pos;
	camera->dir = dir;
	camera->up = up;
	
	camera->CalculateView();
	
	if (windowEvent) {
		camera->aspect = float(windowWidth) / float(windowHeight);
		camera->CalculatePerspective();
	}
}

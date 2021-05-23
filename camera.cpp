#include "camera.h"

static const float speed = 0.0010f; // 1 units / second
static const float mouseSpeed = 0.005f;

void Camera::Update(InputHandler * input)
{
	// Mouse
	
	dir = glm::normalize(dir);
	glm::vec3 right = glm::cross(dir, up);

	int axrel = abs(input->xrel);
	int ayrel = abs(input->yrel);

	if (!input->buttonMask) {
	// Yaw (left and right)
		dir = glm::normalize((dir * float(100 - axrel)) + (right * float(input->xrel)));
	}
	else if (input->buttonMask == SDL_BUTTON_RMASK) {
	// Roll (like rolling your head)
		up = glm::normalize((up * float(100 - axrel)) + (right * float(input->xrel)));
	}
	right = glm::cross(dir, up);

	// Pitch (up and down)
	dir = glm::normalize((dir * float(100 - ayrel)) + (up * float(input->yrel)));
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

	CalculateView();
	
	if (input->windowEvent) {
		aspect = float(input->windowWidth) / float(input->windowHeight);
		CalculatePerspective();
	}
}

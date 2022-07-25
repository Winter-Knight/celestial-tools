#include <cstdio>
#include <chrono>
#include "world.h"
#include "window.h"
#include "groundplane.h"
#include "parser.h"

#ifndef DEFAULT_SYSTEM_FILE
	#define DEFAULT_SYSTEM_FILE "../../examples/sol.txt"
#endif

Window * window;
InputHandler * input;
World * world;
GroundPlane groundplane;

const char * systemFile;

namespace stdtime = std::chrono;

long long nowMicro()
{
	stdtime::system_clock::time_point tp = stdtime::system_clock::now();
	stdtime::system_clock::duration   d  = tp.time_since_epoch();
	stdtime::microseconds             us = stdtime::duration_cast<stdtime::microseconds> (d);
	return us.count();
}

stdtime::duration<float> getDelta()
{
	static stdtime::steady_clock::time_point lastTime = stdtime::steady_clock::now();
	stdtime::steady_clock::time_point now = stdtime::steady_clock::now();
	stdtime::duration<float> d = now - lastTime;
	lastTime = now;
	return d;
}

void play()
{
//	long long time = 31557600000000; // one year after epoch
	long long time = 0;
	Camera * camera = world->GetCamera();
	glm::mat4 rotationMatrix;
	int celestialID = -1;
//	float timeSpeed = 10000.0f;
	float timeSpeed = 1.0f;
	const unsigned char * keyboardState = SDL_GetKeyboardState(NULL);
	while (!input->HandleEvents(window)) {
		if (!input->paused)
			time += getDelta().count() * 1000000.0f * timeSpeed;
		world->Update(time);

		if (input->lastKey == SDLK_F6)
			celestialID = (celestialID == 3) ? -1 : 3; /* Rosa or -1 */
		if (input->lastKey == SDLK_SEMICOLON)
			printf("[%5.2f][%5.2f][%5.2f]\n", camera->pos.x, camera->pos.y, camera->pos.z);
		if (keyboardState[SDL_SCANCODE_KP_PLUS] || keyboardState[SDL_SCANCODE_EQUALS]) {
			timeSpeed += 100.0f;
			printf("time speed: %f\n", timeSpeed);
		}
		if (keyboardState[SDL_SCANCODE_KP_MINUS] || keyboardState[SDL_SCANCODE_MINUS]) {
			timeSpeed -= 100.0f;
			printf("time speed: %f\n", timeSpeed);
		}
		if (input->lastKey == SDLK_HOME) {
			*camera = *getCameraFromFile(systemFile);
			camera->CalculateView();
			camera->aspect = float(input->windowWidth) / float(input->windowHeight);
			camera->CalculatePerspective();
		}
		if (input->lastKey == SDLK_g) {
			if (SDL_GetRelativeMouseMode())
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		static GLenum wireframe = GL_FILL;
		if (input->lastKey == SDLK_z) {
			wireframe = (wireframe == GL_FILL) ? GL_LINE : GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe);
		}

		window->Clear();
		world->DrawPerspective(celestialID);
		if (celestialID == 3)
			groundplane.Draw(camera);
		window->Swap();
	}
}

int main(int argc, char ** argv)
{
	if (argc < 2)
		systemFile = DEFAULT_SYSTEM_FILE;
	else
		systemFile = argv[1];

	window = new Window();
	input = new InputHandler();
	world = new World(window, input);
	world->Init(systemFile);
	groundplane.Init();

	play();

	return 0;
}

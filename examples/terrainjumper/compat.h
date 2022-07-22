#ifdef USE_GLFW
	
	#include "glfw/window.h"
	#include "glfw/input.h"
	
	#define Event int
	#define PollEvent(x) input->PollEvent(x)
	
	#define GetKeyboardState(x) InputHandler::GetKeyStates(x)

	#define KEY_SPACE GLFW_KEY_SPACE
	#define LSHIFT    GLFW_KEY_LEFT_SHIFT
	#define RSHIFT    GLFW_KEY_RIGHT_SHIFT
	#define LALT      GLFW_KEY_LEFT_ALT
	#define RALT      GLFW_KEY_RIGHT_ALT
	#define KEY_W     GLFW_KEY_W
	#define KEY_S     GLFW_KEY_S
	#define KEY_A     GLFW_KEY_A
	#define KEY_D     GLFW_KEY_D

#else // Use SDL by default

	#include "sdl/window.h"
	#include "sdl/input.h"
	
	#define Event SDL_Event
	#define PollEvent(x) SDL_PollEvent(x)

	#define GetKeyboardState(x) SDL_GetKeyboardState(x)
	#define KEY_SPACE SDLK_SPACE
	#define LSHIFT SDL_SCANCODE_LSHIFT
	#define RSHIFT SDL_SCANCODE_RSHIFT
	#define LALT   SDL_SCANCODE_LALT
	#define RALT   SDL_SCANCODE_RALT
	#define KEY_W  SDL_SCANCODE_W
	#define KEY_S  SDL_SCANCODE_S
	#define KEY_A  SDL_SCANCODE_A
	#define KEY_D  SDL_SCANCODE_D

#endif

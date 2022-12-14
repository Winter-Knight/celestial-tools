#include <cstdio>
#include <string>

#include "window.h"

void Window::SetIcon(const char * filename)
{
	FIBITMAP * icondata = LoadImage(filename);
	FreeImage_FlipVertical(icondata);
	SDL_Surface * iconsurface = SDL_CreateRGBSurfaceFrom(FreeImage_GetBits(icondata), 64, 64, 32, 64 * 4, 0xff0000, 0xff00, 0xff, 0xff000000);

	SDL_SetWindowIcon(window, iconsurface);
	SDL_FreeSurface(iconsurface);
	FreeImage_Unload(icondata);
}

Window::Window()
{
	width = INITIAL_WINDOW_WIDTH;
	height = INITIAL_WINDOW_HEIGHT;


	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow("Solar System", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SetIcon("images/program_icon.png");
	context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	int profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	printf("OpenGL Context Profile Core: %d\n", profile == GL_CONTEXT_CORE_PROFILE_BIT);

	printf("SDL Compiled Version: %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
	SDL_version version;
	SDL_GetVersion(&version);
	printf("SDL Linked Version: %d.%d.%d\n", version.major, version.minor, version.patch);

// #ifdef _WIN32
// Debian ImGui needs glew
	GLenum err = glewInit();
	glGetError(); // GLEW triggers OpenGL error if core profile is set. Discard GLEW errors.
	if (err != GLEW_OK) {
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("Glew Error: %s\n", glewGetErrorString(err));
	}
	printf("Glew Version: %s\n", glewGetString(GLEW_VERSION));
// #endif // _WIN32

	CheckSDLError("Window()");

	InitOpenGL();
}

void Window::InitOpenGL()
{
	glClearColor(0.01f, 0.01f, 0.05f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CheckGLError("Window()");
}

Window::~Window()
{
	CheckSDLError("~Window()");
	CheckGLError("~Window()");

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Window::CheckSDLError(const char * str)
{
	const char * error = SDL_GetError();
	if (*error) {
		printf("%s - SDL Error: %s\n", str, error);
		SDL_ClearError();
	}
	return !!error;
}

int Window::CheckGLError(const char * str)
{
	static std::string lastStr;
	static GLenum lastError = GL_NO_ERROR;

	GLenum error = glGetError();
	
	if (lastStr == str && lastError == error)
		return error;

	if (error) {
		printf("%s - OpenGL Error: %04x\n", str, error);
		lastStr = str;
		lastError = error;
	}
	return error;
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Swap()
{
	SDL_GL_SwapWindow(window);

	CheckSDLError("Update()");
	CheckGLError("Update()");
}

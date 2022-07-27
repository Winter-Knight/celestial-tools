#include <cstdio>
#include <string>

#include "window.h"

void Window::SetIcon(const char * filename)
{
	FIBITMAP * icondata = LoadImage(filename);
	FreeImage_FlipVertical(icondata);

	GLFWimage image;
	image.width = image.height = 64;
	image.pixels = (unsigned char *) FreeImage_GetBits(icondata);
	
	for (unsigned int i = 0; i < 64 * 64; i++) {
		unsigned int blue = ((unsigned int *) image.pixels)[i] & 0xff;
		unsigned int green = (((unsigned int *) image.pixels)[i] & 0xff00) >> 8;
		unsigned int red = (((unsigned int *) image.pixels)[i] & 0xff0000) >> 16;
		unsigned int alpha = 0xff;
		((unsigned int *) image.pixels)[i] = (alpha << 24) | (blue << 16) | (green << 8) | (red);
	}

	glfwSetWindowIcon(window, 1, &image);
	FreeImage_Unload(icondata);
}

Window::Window()
{
	width = INITIAL_WINDOW_WIDTH;
	height = INITIAL_WINDOW_HEIGHT;

	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "Solar System", NULL, NULL);

	// Center window
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * mode = glfwGetVideoMode(monitor);
	int x = (mode->width - width) / 2;
	int y = (mode->height - height) / 2;
	glfwSetWindowPos(window, x, y);

	glfwShowWindow(window);
	SetIcon("images/program_icon.png");
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	int profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	printf("OpenGL Context Profile Core: %d\n", profile == GL_CONTEXT_CORE_PROFILE_BIT);

	printf("GLFW Compiled Version: %d.%d.%d\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
	int version_major, version_minor, version_revision;
	glfwGetVersion(&version_major, &version_minor, &version_revision);
	printf("GLFW Linked Version: %d.%d.%d\n", version_major, version_minor, version_revision);
	
	GLenum err = glewInit();
	glGetError(); // GLEW triggers OpenGL error if core profile is set. Discard GLEW errors.
	if (err != GLEW_OK) {
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("Glew Error: %s\n", glewGetErrorString(err));
	}
	printf("Glew Version: %s\n", glewGetString(GLEW_VERSION));

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
	CheckGLError("~Window()");

	glfwDestroyWindow(window);
	glfwTerminate();
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
	glfwSwapBuffers(window);

	CheckGLError("Update()");
}

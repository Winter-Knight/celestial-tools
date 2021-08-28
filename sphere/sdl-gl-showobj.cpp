#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../mvp2.h"
#include "../../fps-controls2.h"

#include "sphere.h"

const unsigned int BUFFER_SIZE = 32768;
const unsigned int INITIAL_WINDOW_WIDTH = 1200;
const unsigned int INITIAL_WINDOW_HEIGHT = 900;

typedef struct {
	glm::vec3 * vertices;
	glm::vec3 * uvs;
	glm::vec3 * normals;
	glm::vec3 * tangents;
	glm::vec3 * bitangents;
	std::vector<unsigned short> indices;
	
	unsigned short numVertices;
	unsigned short numUVs;
	unsigned short numNormals;
	unsigned short numTangents;
	unsigned short numBitangents;
	unsigned short numIndices;
} Mesh;

MVPHandler mvphandler;
Control control;

SDL_Window * window;

Mesh mesh;

int paused = 0;
int quit = 0;
int wireframe = 0;
unsigned int windowWidth = INITIAL_WINDOW_WIDTH;
unsigned int windowHeight = INITIAL_WINDOW_HEIGHT;

GLenum CheckGLError(const char * str = "")
{
	GLenum error = glGetError();
	if (error)
		printf("%s - OpenGL Error: 0x%04x\n", str, error);
	return error;
}

int CheckSDLError(const char * str = "")
{
	const char * error = SDL_GetError();
	if (*error) {
		printf("%s - SDL Error: %s\n", str, error);
		SDL_ClearError();
	}
	return !!*error;
}

int CheckGLShader(GLuint shader, const char * str = "")
{
	int status, len;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	printf("%s - OpenGL Shader Compile Status: %d\n", str, status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 1) {
		GLchar buffer[BUFFER_SIZE];
		glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, buffer);
		printf("%s - OpenGL Shader Info Log:\n%s\n", str, buffer);
	}
	return !status;
}

int CheckGLProgram(GLuint program, const char * str = "")
{
	int validateStatus, linkStatus, len;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus);
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	int error = (!validateStatus || !linkStatus || len > 1);
	if (error) {
		printf("%s - OpenGL Program Status: %d %d %d\n", str, validateStatus, linkStatus, len);
		GLchar buffer[BUFFER_SIZE];
		glGetProgramInfoLog(program, BUFFER_SIZE, NULL, buffer);
//		printf("%s - OpenGL Program Info Log:\n%s\n", str, buffer);
	}
	return error;
}

const char * LoadModel(Mesh * model)
{
	unsigned int i, j;
	
	Sphere sphere1, sphere2;
	sphere1.InitOctahedron();
	for (i = 0; i < 3; i++) {
		if (i % 2 == 0)
			sphere2 = sphere1.Quadruple();
		else
			sphere1 = sphere2.Quadruple();
	}
	static Sphere sphere = (i % 2 == 0) ? sphere1.Quadruple() : sphere2.Quadruple();

	model->vertices = sphere.vertices.data();
	model->numVertices = sphere.vertices.size();

	for (i = 0; i < sphere.faces.size(); i++) {
		glm::uvec3 face = sphere.faces[i];
		for (j = 0; j < 3; j++)
			model->indices.push_back(face[j]);
	}
	model->numIndices = i * j;

	return NULL;
}

void AddShader(GLuint program, const char * shaderfile, GLenum shadertype)
{
	FILE * fp;
	int len;
	GLchar shaderbuffer[BUFFER_SIZE];
	const GLchar * pbuffer = shaderbuffer;
	GLuint shader;
	
	if (shaderfile) {
		fp = fopen(shaderfile, "rb");
		len = fread(shaderbuffer, 1, BUFFER_SIZE, fp);
		fclose(fp);
		shaderbuffer[len] = 0;
		
		shader = glCreateShader(shadertype);
		glShaderSource(shader, 1, &pbuffer, NULL);
		glCompileShader(shader);
		CheckGLShader(shader, shaderfile);
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
}

GLuint AddProgram(const char * vertexshaderfile, const char * fragmentshaderfile)
{
	GLuint program = glCreateProgram();
	
	AddShader(program, vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(program, fragmentshaderfile, GL_FRAGMENT_SHADER);
	
	glLinkProgram(program);
	CheckGLProgram(program, "OpenGL Program");
	return program;
}

GLint GetUniformLocation(const char * name)
{
	GLuint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *) &program);
	return glGetUniformLocation(program, name);
}

void SetUniformi(const char * name, GLint value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void SetUniformf(const char * name, GLfloat value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void SetUniform3f(const char * name, GLfloat * values)
{
	glUniform3fv(GetUniformLocation(name), 1, values);
}

void SetUniformMatrix4f(const char * name, GLfloat * values)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, values);
}

void init()
{
	CheckGLError("init begin");
	
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GLuint vertexarray;
	glGenVertexArrays(1, &vertexarray);
	glBindVertexArray(vertexarray);

	const char * error = LoadModel(&mesh);
	if (error) {
		printf("Error loading mesh!\n");
		exit(1);
	}
	
	printf("numVertices: %d\n", mesh.numVertices);
	printf("numFaces: %d\n", mesh.numIndices / 3);
	
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(glm::vec3), mesh.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	GLuint program = AddProgram("showobj.vert", "../../sdl-gl/shaders/simple.frag");
	glUseProgram(program);
	
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndices * sizeof(unsigned short), mesh.indices.data(), GL_STATIC_DRAW);

	// MVPHandler+
	mvphandler.model = glm::mat4(1.0f);
	mvphandler.pos = glm::vec3(0.0f, 0.0f, 50.0f);
	mvphandler.dir = glm::vec3(0.0f, 0.0f, -1.0f);
	mvphandler.up = glm::vec3(0.0f, 1.0f, 0.0f);
	mvphandler.fov = glm::radians(45.0f);
	mvphandler.aspect = float(windowWidth) / float(windowHeight);
	mvphandler.near = 0.1f;
	mvphandler.far = 100.0f;
	mvphandler.CalculatePerspective();

	srand(time(NULL));
	
	if (!CheckGLError("end of init"))
		printf("init completed without error\n");
}

void SetupMesh()
{
	mvphandler.CalculateView();
	mvphandler.CalculateMVP();
	SetUniformMatrix4f("MVP", &mvphandler.MVP[0][0]);
	
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CheckGLError("display");
	CheckSDLError("display");
	
	SetupMesh();
	glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_SHORT, (void *) 0);
	
	SDL_GL_SwapWindow(window);
}

void keyboard(SDL_Event * event)
{
	switch(event->key.keysym.sym) {
		case SDLK_ESCAPE:
			quit = 1;
			break;
		case SDLK_SPACE:
			paused = !paused;
			break;
		case SDLK_z:
			wireframe = !wireframe;
			break;
	}
}

void mouseMotion(SDL_Event * event)
{
	if (event->motion.windowID == SDL_GetWindowID(window))
		control.mouseMove(&mvphandler, event->motion.xrel, event->motion.yrel, event->motion.state);
	else
		SDL_ClearError();
}

void windowEvent(SDL_Event * event)
{
	if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		windowWidth = event->window.data1;
		windowHeight = event->window.data2;
		mvphandler.aspect = float(windowWidth) / float(windowHeight);
		mvphandler.CalculatePerspective();
		glViewport(0, 0, windowWidth, windowHeight);
	}
}

void mainLoop()
{
	SDL_Event event;
	unsigned int currentTime = SDL_GetTicks();
	unsigned int lastTime;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = 1;
			if (event.type == SDL_KEYDOWN)
				keyboard(&event);
			if (event.type == SDL_MOUSEMOTION)
				mouseMotion(&event);
			if (event.type == SDL_WINDOWEVENT)
				windowEvent(&event);
		}
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		control.keyboard(&mvphandler, currentTime - lastTime);
		display();
	}
}

int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	window = SDL_CreateWindow("SDL GL Demo Graphics Program", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	int profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	printf("OpenGL Core Profile: %d\n", profile == GL_CONTEXT_CORE_PROFILE_BIT);
	
	printf("SDL Compiled Version: %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
	SDL_version version;
	SDL_GetVersion(&version);
	printf("SDL Linked Version: %d.%d.%d\n", version.major, version.minor, version.patch);
	
	CheckSDLError("init SDL");
	init();
	mainLoop();
	CheckSDLError("end SDL");
	
	printf("Thank you for playing Wing Commander!\n");
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

#include "world.h"
#include "parser.h"

void World::Init(const char * systemFile)
{
	// Setup camera
	int width, height;
	SDL_GetWindowSize(window->GetWindow(), &width, &height);
	input->windowWidth = width;
	input->windowHeight = height;

	if (!(camera = getCameraFromFile(systemFile))) {
		input->quit = 1;
		return;
	}
	camera->CalculateView();
	camera->aspect = float(width) / float(height);
	camera->CalculatePerspective();

	// Setup skybox
	skybox = new Skybox();
	skybox->dirName = "images/starry-sky";
	skybox->Init();
	skybox->program = new GLProgram("shaders/skybox.vert", "shaders/skybox.frag");
	
	// Setup skypanorama
	skypanorama = new SkyPanorama();
	skypanorama->Init(getStarboxFromFile(systemFile).c_str());
	skypanorama->program = new GLProgram("shaders/skypanorama.vert", "shaders/skypanorama.frag");
	
	// OpenGL Shapes
	Celestial::sphere.Init(4);
	Celestial::plane.Init();

	// Load celestial objects
//	parseSystemFile(celestials, systemFile, "shaders/celestial.vert", "shaders/celestial.tes");
	parseSystemFile(celestials, systemFile);
	
	// Init light array (array of celestial bodies that emit light)
	for (unsigned int i = 0; i < celestials.size(); i++)
		if (celestials[i]->light.r > 0.0f || celestials[i]->light.g > 0.0f || celestials[i]->light.b > 0.0f)
			Celestial::lightArray.indexList.push_back(i);
		

	// Set up debugging:
	
	debugProgram = new GLProgram("shaders/celestialV.vert", "shaders/gas-debug.frag");
	debugFramebuffer = new Framebuffer();
	debugFramebuffer->Init(3, 3, GL_FLOAT, 2048, 2048);

	glDisable(GL_CULL_FACE);
}

void World::Update(long long time)
{
	for (unsigned int i = 0; i < celestials.size(); i++)
		celestials[i]->Update(input, time);

	camera->Update(input);

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

	// Update light array (array of celestial bodies that emit light)
	Celestial::lightArray.ClearLights();
	for (unsigned int i = 0; i < Celestial::lightArray.indexList.size(); i++) {
		unsigned int index = Celestial::lightArray.indexList[i];
		Celestial::lightArray.AddLight(glm::vec4(celestials[index]->pos, 0.0f), celestials[index]->light);
	}
}

#include "../gl-debug.h"

void World::DrawPerspective(int celestialID)
{
	static GLProgram marioCPUSphereProgram = *celestials[1]->program;
	static GLProgram marioTessSphereProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/mario.frag");
	static SphereType sphereType = CPUSPHERE;

	if (input->lastKey == SDLK_o) {
		if (sphereType == TESSSPHERE) {
			sphereType = CPUSPHERE;
			celestials[1]->program = &marioCPUSphereProgram;
		}
		else if (sphereType == CPUSPHERE) {
			sphereType = TESSSPHERE;
			celestials[1]->program = &marioTessSphereProgram;
		}
	}
	
	// Increase or decrease the tilt of Rosa for debugging
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP])
		celestials[3]->tilt += 0.1f;
	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN])
		celestials[3]->tilt -= 0.1f;

	Camera drawCam;
	if (celestialID >= 0) {
		drawCam.perspective = camera->perspective;
		drawCam.pos = celestials[celestialID]->pos;
		drawCam.dir = camera->dir;
		drawCam.up = camera->up;
		drawCam.CalculateView();
	
		glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(celestials[celestialID]->tilt), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), celestials[celestialID]->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Rotations = R1 * R2;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), celestials[celestialID]->pos);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f), drawCam.dir, drawCam.up);
	//	drawCam.view = Rotations * drawCam.view;
	//	drawCam.view = drawCam.view * Rotations;
		drawCam.view = view * glm::inverse(Rotations) * glm::inverse(T);
	}
	else
		drawCam = *camera;

	// Draw all objects
	skypanorama->Draw(&drawCam);
	for (unsigned int i = 0; i < celestials.size(); i++)
		if ((int) i != celestialID)
			celestials[i]->Draw(&drawCam, sphereType);
}

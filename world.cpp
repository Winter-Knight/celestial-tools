#include "world.h"
#include "parser.h"

void World::Init(const char * systemFile)
{
	// Setup camera
	int width, height;
	SDL_GetWindowSize(window->GetWindow(), &width, &height);
	input->windowWidth = width;
	input->windowHeight = height;

	camera = getCameraFromFile(systemFile);
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
	skypanorama->Init("starbox-creator/starfield3.png");
	skypanorama->program = new GLProgram("shaders/skypanorama.vert", "shaders/skypanorama.frag");
	
	// OpenGL Sphere
	Celestial::sphere.Init(4);

	// Load celestial objects
//	parseSystemFile(celestials, systemFile, "shaders/celestial.vert", "shaders/celestial.tes");
	parseSystemFile(celestials, systemFile);

	// Set up debugging:
	
	debugProgram = new GLProgram("shaders/celestialV.vert", "shaders/gas-debug.frag");
	debugFramebuffer = new Framebuffer();
	debugFramebuffer->Init(3, 3, GL_FLOAT, 2048, 2048);

	glDisable(GL_CULL_FACE);
}

void World::Play()
{
	unsigned int i;
	float time = 0.0f;
	
	GLProgram marioCPUSphereProgram = *celestials[1]->program;
	GLProgram marioTessSphereProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/gas.frag");
	SphereType sphereType = CPUSPHERE;

	while (!input->HandleEvents(window)) {
		if (!input->paused)
			time += 16.0f / (60.0f * 60.0f * 1000.0f);
		for (i = 0; i < celestials.size(); i++)
			celestials[i]->Update(input, time);

		camera->Update(input);

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

		window->Clear();

		// Draw all objects
		for (i = 0; i < celestials.size(); i++)
//			celestials[i]->Draw(camera, CPUSPHERE);
			celestials[i]->Draw(camera, sphereType);

//		skybox->Draw(camera);
//		skypanorama->Draw(camera);

		window->Swap();

		// Now write into debug buffer
		
		debugFramebuffer->Use();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Celestial::sphere.Bind();
		
		debugProgram->Use();

		glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(celestials[1]->tilt), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), celestials[1]->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Rotations = R1 * R2;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), celestials[1]->pos);
		glm::mat4 VP = camera->perspective * camera->view;

		debugProgram->SetUniformMatrix4f("VP", &VP[0][0]);
		debugProgram->SetUniformMatrix4f("Rotations", &Rotations[0][0]);
		debugProgram->SetUniformMatrix4f("Translation", &T[0][0]);

		debugProgram->SetUniform3f("centerPos_worldspace", &celestials[1]->pos[0]);
		debugProgram->SetUniformf("size", celestials[1]->size);

		for (unsigned int i = 0; i < celestials[1]->textures.size(); i++)
			celestials[1]->textures[i].Bind(debugProgram, i);

		glDrawElements(GL_TRIANGLES, Celestial::sphere.numIndices, GL_UNSIGNED_INT, 0);

		debugFramebuffer->CheckForClicks(input);
		debugFramebuffer->Stop();

		if (input->lastKey == SDLK_g) {
			if (SDL_GetRelativeMouseMode())
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		
		static int wireframe = 0;
		if (input->lastKey == SDLK_z) {
			wireframe = !wireframe;
			if (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
			
	}
}

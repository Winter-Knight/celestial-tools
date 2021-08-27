#include "world.h"
#include "parser.h"

void World::Init(const char * systemFile)
{
	// Setup camera
	int width, height;
	SDL_GetWindowSize(window->GetWindow(), &width, &height);

	camera = getCameraFromFile(systemFile);
	camera->CalculateView();
	camera->aspect = float(width) / float(height);
	camera->CalculatePerspective();

	printf("width: %d, height: %d, aspect: %f\n", width, height, camera->aspect);

	// Setup skybox
	skybox = new Skybox();
	skybox->dirName = "images/starry-sky";
	skybox->Init();
	skybox->program = new GLProgram("shaders/skybox.vert", "shaders/skybox.frag");

	parseSystemFile(celestials, systemFile);
}

void World::Play()
{
	unsigned int i;
	float time = 0.0f;

	while (!input->HandleEvents(window)) {
		if (!input->paused)
			time += 16.0f / (60.0f * 60.0f * 1000.0f);
		for (i = 0; i < celestials.size(); i++)
			celestials[i]->Update(input, time);

		camera->Update(input);

		window->Clear();

		// Draw all objects
		for (i = 0; i < celestials.size(); i++)
			celestials[i]->Draw(camera);

		skybox->Draw(camera);

		window->Swap();
	}
}

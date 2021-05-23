#include "world.h"

void World::Init()
{
	// Setup camera
	camera = new Camera();
	camera->pos = glm::vec3(778.0f, 0.0f, 1.5f);
	camera->dir = glm::vec3(0.0f, 0.0f, -1.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->CalculateView();

	int width, height;
	SDL_GetWindowSize(window->GetWindow(), &width, &height);
	camera->fov = glm::radians(60.0f);
	camera->aspect = float(width) / float(height);
	camera->near = 0.001f;
	camera->far = 10000.0f;
	camera->CalculatePerspective();
	printf("width: %d, height: %d, aspect: %f\n", width, height, camera->aspect);

	// Setup skybox
	skybox = new Skybox();
	skybox->dirName = "images/starry-sky";
	skybox->Init();
	skybox->program = new GLProgram("shaders/skybox.vert", "shaders/skybox.frag");

	// Setup sun
	sun = new Celestial();
	sun->pos = glm::vec3(0.0f);
	sun->size = 6.960f; // multiplied by 10
	sun->Init();
	sun->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/sun.frag");
	
	// Gas Giant
	planet = new Orbital();
	planet->distance = 778.0f;
	planet->period = 8760.0f; // 1 year = 1 year
	planet->tilt = 45.0f;
	planet->size = 0.069911f;
	planet->Init();
	planet->orbitObject = sun;
	planet->AddTexture("images/gas_giant_lookup1.png");
	planet->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/gas.frag");
	
	// Moons
	shadow = new Orbital();
	shadow->distance = 0.4218f;
	shadow->eccentricity = 0.0041f;
	shadow->period = 1.0f / 120.0f; // 1 moon cycle = 30 seconds
	shadow->size = 0.036306f; // multiplied by 10
	shadow->Init();
	shadow->orbitObject = planet;
	shadow->AddTexture("images/callisto_4k_map_by_jcpag2010_dc4fjip.png");
	shadow->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/shadow.frag");

	rosa = new Orbital();
	rosa->distance = 0.6711f;
	rosa->eccentricity = 0.0094f;
	rosa->period = 1.0f / 60.0f; // 1 moon cycle = 1 minute
	rosa->size = 0.031216; // multiplied by 10
	rosa->Init();
	rosa->orbitObject = planet;
	rosa->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/rosa.frag");
	
	daisy = new Orbital();
	daisy->distance = 1.0704f;
	daisy->eccentricity = 0.0011f;
	daisy->period = 1.0f / 30.0f; // 1 moon cycle = 2 minutes
	daisy->period_offset = daisy->period * 1.0f / 4.0f;
	daisy->tilt = 50.0f;
	daisy->rotation_period = 1.0f / 60.0f / 60.0f; // 1 second
	daisy->size = 0.052682; // multiplied by 10
	daisy->Init();
	daisy->orbitObject = planet;
	daisy->AddTexture("images/io_rgb_cyl.png");
	daisy->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", "shaders/daisy.frag");
}

void World::Play()
{
	while (!input->HandleEvents(window)) {
		float time = float(SDL_GetTicks()) / (60.0f * 60.0f * 1000.0f);
		planet->Update(input, time);
		rosa->Update(input, time);
		shadow->Update(input, time);
		daisy->Update(input, time);

//		camera->pos = rosa->pos;
		camera->Update(input);
		
		window->Clear();

		// Draw all objects
		sun->Draw(camera);
		planet->Draw(camera);
		shadow->Draw(camera);
		daisy->Draw(camera);

		glEnable(GL_CULL_FACE);
		rosa->Draw(camera);
		glDisable(GL_CULL_FACE);

		skybox->Draw(camera);
		
		window->Swap();
	}
}

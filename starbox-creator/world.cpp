#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "world.h"
#include "pngsaver.h"
#include "imguifilebrowser/ImGuiFileBrowser.h"

void World::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup camera
	int width, height;
	SDL_GetWindowSize(window->GetWindow(), &width, &height);
	input->windowWidth = width;
	input->windowHeight = height;

	camera = new Camera();
	camera->pos = glm::vec3(0.0f);
	camera->dir = glm::vec3(0.0f, 0.0f, -1.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->CalculateView();
	
	camera->fov = glm::radians(60.0f);
	camera->aspect = float(width) / float(height);
	camera->near = 100.0f;
	camera->far = 100000.0f;
	camera->CalculatePerspective();
	
	// Init sphere
	Star::sphere.InitIcosahedron();
	Star::sphere = Star::sphere.Quadruple();
	
	// Init StarArray
	starArray.sphereProgram = new GLProgram("shaders/star.vert", "shaders/star.frag");
	starArray.billboardProgram = new GLProgram("shaders/billboards.vert", "shaders/billboards.frag");

	// Options Window
	optionsWindow = new OptionsWindow();
	optionsWindow->Init(window->GetWindow(), window->GetContext());
	
	SDL_SetRelativeMouseMode(SDL_FALSE);

	// Stars
	twister.Init();
	optionsWindow->Load();
	UpdateStars();

	// Set up debugging:
	
	debugProgram = new GLProgram("shaders/billboards-wrap.vert", "shaders/billboards-debug.frag");
	debugFramebuffer = new Framebuffer();
	debugFramebuffer->Init(3, 3, GL_FLOAT, 2048, 2048);

	// Set up PNG Framebuffer
	pngFramebuffer = new Framebuffer();
	pngFramebuffer->Init(1, 4, GL_UNSIGNED_BYTE, 8192, 4096);
}

#define rand distributionDouble11(twister.GetGenerator())

void World::UpdateStars()
{
	Star star;
	
	Options * options = optionsWindow->GetOptions();

	float distance, size;
	glm::vec4 color;

	std::uniform_real_distribution<double> distributionDouble11(-1.0, 1.0);
	std::normal_distribution<double> asdnd(options->averageStarDistance, options->averageStarDistanceDeviation);
	std::normal_distribution<double> assnd(options->averageStarSize, options->averageStarSizeDeviation);
	std::normal_distribution<double> acndr(options->averageColor.r, options->averageColorDeviation.r);
	std::normal_distribution<double> acndg(options->averageColor.g, options->averageColorDeviation.g);
	std::normal_distribution<double> acndb(options->averageColor.b, options->averageColorDeviation.b);
	std::normal_distribution<double> acnda(options->averageColor.a, options->averageColorDeviation.a);

	starArray.ClearStars();

	for (unsigned int i = 0; i < options->numStars; i++) {

		if (options->averageStarDistanceNormalDistribution)
			distance = asdnd(twister.GetGenerator());
		else
			distance = options->averageStarDistance + rand * options->averageStarDistanceDeviation;
		
		if (options->averageStarSizeNormalDistribution)
			size = assnd(twister.GetGenerator());
		else
			size = options->averageStarSize + rand * options->averageStarSizeDeviation;
		
		if (options->averageColorNormalDistribution)
			color = glm::vec4(acndr(twister.GetGenerator()), acndg(twister.GetGenerator()), acndb(twister.GetGenerator()), acnda(twister.GetGenerator()));
		else
			color = options->averageColor + glm::vec4(rand, rand, rand, rand) * options->averageColorDeviation;

		double horzAngle = twister.NextDouble() * glm::two_pi<double>();
		double vertAngle = asin(twister.NextDouble() * 2.0 - 1.0);
		star.pos = glm::vec3(cos(vertAngle) * cos(horzAngle), sin(vertAngle), cos(vertAngle) * sin(horzAngle)) * distance;
		star.size = size;
		star.color = color;
		starArray.AddStar(&star);
	}

	if (options->billboards)
		starArray.Update(BILLBOARDS);
	else
		starArray.Update(SPHERES);
}

void World::Play()
{
	SDL_Event event;
	bool showStarbox = false;
	bool popupFileDialog = false;
	imgui_addons::ImGuiFileBrowser file_dialog;

	while (!input->quit) {

		input->Reset();
		while (SDL_PollEvent(&event)) {
			if (!SDL_GetRelativeMouseMode())
				ImGui_ImplSDL2_ProcessEvent(&event);
			input->HandleEvents(&event, window);
		}
		
		if (input->lastKey == SDLK_F8) {
			if (SDL_GetRelativeMouseMode())
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		
		if (SDL_GetRelativeMouseMode())
			camera->Update(input);

		if (optionsWindow->GetActions()->updateStars || input->lastKey == SDLK_F5)
			UpdateStars();

		if (optionsWindow->GetActions()->saveStarbox)
			popupFileDialog = true;
		
		if (optionsWindow->GetActions()->previewStarbox)
			showStarbox = !showStarbox;


		window->Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->GetWindow());
		ImGui::NewFrame();

		if (showStarbox) {
			StarType starType = optionsWindow->GetOptions()->billboards ? BILLBOARDS : SPHERES;

			// Draw PNG to screen (preview)
			starArray.SaveToPNG(starType);
			optionsWindow->Draw(0.016f);
			
			debugFramebuffer->Use();
			window->Clear();
			debugProgram->Use();
			glDrawElements(GL_TRIANGLES, starArray.stars.size() * 6, GL_UNSIGNED_INT, 0);
			debugFramebuffer->CheckForClicks(input);
			debugFramebuffer->Stop();
		}
		else {
			starArray.Draw(camera);
			optionsWindow->Draw(0.016f);
		}

		if (popupFileDialog) {
			ImGui::OpenPopup("Save File...");
			popupFileDialog = false;
		}
		if (file_dialog.showFileDialog("Save File...", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".png")) {
			StarType starType = optionsWindow->GetOptions()->billboards ? BILLBOARDS : SPHERES;

			pngFramebuffer->Use();
			glViewport(0, 0, 8192, 4096);
			
			// Draw PNG to framebuffer
			window->Clear();
			starArray.SaveToPNG(starType);

			unsigned char * imageData = (unsigned char *) pngFramebuffer->GetData(0);

			pngFramebuffer->Stop();
			glViewport(0, 0, input->windowWidth, input->windowHeight);

			SaveToPNG(file_dialog.selected_path.c_str(), 4, 8, 8192, 4096, imageData);

			delete[] imageData;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window->Swap();
	}
	
	optionsWindow->Save();
}
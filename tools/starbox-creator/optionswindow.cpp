#include <fstream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>

#include "optionswindow.h"

const char * optionsFile = ".starbox-creator-options";

void OptionsWindow::Init(SDL_Window * w, SDL_GLContext * context)
{
	window = w;

	// Init ImGui
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui_ImplSDL2_InitForOpenGL(window, *context);
}

void OptionsWindow::Draw(float delta)
{
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	ImGuiIO & io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowWidth, windowHeight);
	io.DeltaTime = glm::min(delta, 0.001f);

	ImGui::Begin("Options");

	ImGui::InputScalar("Seed", ImGuiDataType_U32, &options.seed);
	ImGui::DragScalar("Number of Stars", ImGuiDataType_U32, &options.numStars, 10.0f);
	ImGui::Checkbox("Billboards", &options.billboards);

	ImGui::Separator();
	ImGui::Text("Average Star Size");
	ImGui::Checkbox("Normal Distribution (size)", &options.averageStarSizeNormalDistribution);
	ImGui::InputFloat("Size", &options.averageStarSize);
	ImGui::InputFloat("Deviation (size)", &options.averageStarSizeDeviation);

	ImGui::Separator();
	ImGui::Text("Average Star Distance");
	ImGui::Checkbox("Normal Distribution (distance)", &options.averageStarDistanceNormalDistribution);
	ImGui::DragFloat("Distance", &options.averageStarDistance, 10.0f);
	ImGui::DragFloat("Deviation (distance)", &options.averageStarDistanceDeviation, 2.0f);

	ImGui::Separator();
	ImGui::Text("Average Star Color");
	ImGui::Checkbox("Normal Distribution (color)", &options.averageColorNormalDistribution);
	ImGui::ColorEdit4("Average Color", (float *) &options.averageColor);
	ImGui::ColorEdit4("Deviation (color)", (float *) &options.averageColorDeviation);
	
	ImGui::Separator();
	int imageSizeIndex;
	switch(options.imageSize) {
		case 2048: imageSizeIndex = 1; break;
		case 4096: imageSizeIndex = 2; break;
		case 8192: imageSizeIndex = 3; break;
		case 16384: imageSizeIndex = 4; break;
		case 1024: default: imageSizeIndex = 0;
	}
	const char * imageSizeItems[] = { "1K", "2K", "4K", "8K", "16K" };

	ImGui::Combo("Image Size", &imageSizeIndex, imageSizeItems, 5);
	switch(imageSizeIndex) {
		case 0: options.imageSize = 1024; break;
		case 1: options.imageSize = 2048; break;
		case 2: options.imageSize = 4096; break;
		case 3: options.imageSize = 8192; break;
		case 4: options.imageSize = 16384; break;
	}

	ImGui::Separator();
	actions.updateStars = ImGui::Button("Update Stars (F5)");
	ImGui::SameLine();
	actions.previewStarbox = ImGui::Checkbox("Preview Skybox (F6)", &options.previewStarbox);
	ImGui::SameLine();
	actions.saveStarbox = ImGui::Button("Save Skybox");

	ImGui::End();
}

void OptionsWindow::Load()
{
	memset(&options, 0, sizeof(options));
	memset(&actions, 0, sizeof(actions));

	std::string line, fieldName;
	std::stringstream ss;
	
	std::string strValue;
	float fValue;
	glm::vec4 floats;
	int iValue;
	unsigned int uValue;

	// Open file
	char * optionsPath = SDL_GetPrefPath("", "starbox-creator");
	std::ifstream file((std::string(optionsPath) + optionsFile).c_str());
	SDL_free(optionsPath);
	if (!file.is_open()) {
		return;
	}

	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;
	
		// Get next word from line, which represents name of field. If field is not reconized continue to next line
		// Get next word from line, which represents value of field.
		ss.clear();
		ss.str(line);
		ss >> fieldName;
		if (fieldName == "seed:")
			ss >> options.seed;
		if (fieldName == "numStars:")
			ss >> options.numStars;
		if (fieldName == "billboards:")
			ss >> options.billboards;

		else if (fieldName == "averageStarSizeNormalDistribution:")
			ss >> options.averageStarSizeNormalDistribution;
		else if (fieldName == "averageStarSize:")
			ss >> options.averageStarSize;
		else if (fieldName == "averageStarSizeDeviation:")
			ss >> options.averageStarSizeDeviation;

		else if (fieldName == "averageStarDistanceNormalDistribution:")
			ss >> options.averageStarDistanceNormalDistribution;
		else if (fieldName == "averageStarDistance:")
			ss >> options.averageStarDistance;
		else if (fieldName == "averageStarDistanceDeviation:")
			ss >> options.averageStarDistanceDeviation;

		else if (fieldName == "averageColorNormalDistribution:")
			ss >> options.averageColorNormalDistribution;
		else if (fieldName == "averageColor:") {
			ss >> floats[0] >> floats[1] >> floats[2] >> floats[3];
			options.averageColor = floats;
		}
		else if (fieldName == "averageColorDeviation:") {
			ss >> floats[0] >> floats[1] >> floats[2] >> floats[3];
			options.averageColorDeviation = floats;
		}
		
		else if (fieldName == "imageSize:")
			ss >> options.imageSize;
	}
	
	if (!options.imageSize)
		options.imageSize = 1024;
	
	file.close();
}

const char * OptionsWindow::GetSaveText()
{
	static std::string saveText;
	std::stringstream ss;

	ss << "seed: " << options.seed;
	ss << "\nnumStars: " << options.numStars;
	ss << "\nbillboards: " << options.billboards;
	
	ss << "\naverageStarSizeNormalDistribution: " << options.averageStarSizeNormalDistribution;
	ss << "\naverageStarSize: " << options.averageStarSize;
	ss << "\naverageStarSizeDeviation: " << options.averageStarSizeDeviation;

	ss << "\naverageStarDistanceNormalDistribution: " << options.averageStarDistanceNormalDistribution;
	ss << "\naverageStarDistance: " << options.averageStarDistance;
	ss << "\naverageStarDistanceDeviation: " << options.averageStarDistanceDeviation;
	
	ss << "\naverageColorNormalDistribution: " << options.averageColorNormalDistribution;
	ss << "\naverageColor: " << options.averageColor.r << " " <<
		options.averageColor.g << " " << options.averageColor.b << " " << options.averageColor.a;
	ss << "\naverageColorDeviation: " << options.averageColorDeviation.r << " " <<
		options.averageColorDeviation.g << " " << options.averageColorDeviation.b << " " << options.averageColorDeviation.a;
	
	ss << "\nimageSize: " << options.imageSize << std::endl;
	
	saveText = ss.str();
	return saveText.c_str();
}	

void OptionsWindow::Save()
{
	char * optionsPath = SDL_GetPrefPath("", "starbox-creator");
	FILE * fp = fopen((std::string(optionsPath) + optionsFile).c_str(), "w");
	SDL_free(optionsPath);
	
	if (!fp) {
		printf("ERROR! CANNOT OPEN OPTIONS FILE FOR WRITING: %s\n", optionsFile);
		return;
	}

	fprintf(fp, "%s", GetSaveText());
	
	fclose(fp);
}

OptionsWindow::~OptionsWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

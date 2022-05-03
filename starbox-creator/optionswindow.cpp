#include <fstream>
#include <sstream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>

#include "optionswindow.h"

// TODO: Should determine home directory another way
const char * optionsFile = "/home/wk/.starbox-creator-options";

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

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	ImGui::Begin("Options");

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
	actions.updateStars = ImGui::Button("Update Stars (F5)");
	ImGui::SameLine();
	actions.saveStarbox = ImGui::Button("Save Skybox");

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	std::ifstream file(optionsFile);
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
			
	}
	
	file.close();
}

void OptionsWindow::Save()
{
	FILE * fp = fopen(optionsFile, "w");
	
	if (!fp) {
		printf("ERROR! CANNOT OPEN OPTIONS FILE FOR WRITING: %s\n", optionsFile);
		return;
	}

	fprintf(fp, "numStars: %u\n", options.numStars);
	fprintf(fp, "billboards: %d\n", options.billboards);

	fprintf(fp, "averageStarSizeNormalDistribution: %d\n", options.averageStarSizeNormalDistribution);
	fprintf(fp, "averageStarSize: %f\n", options.averageStarSize);
	fprintf(fp, "averageStarSizeDeviation: %f\n", options.averageStarSizeDeviation);
	
	fprintf(fp, "averageStarDistanceNormalDistribution: %d\n", options.averageStarDistanceNormalDistribution);
	fprintf(fp, "averageStarDistance: %f\n", options.averageStarDistance);
	fprintf(fp, "averageStarDistanceDeviation: %f\n", options.averageStarDistanceDeviation);
	
	fprintf(fp, "averageColorNormalDistribution: %d\n", options.averageColorNormalDistribution);
	fprintf(fp, "averageColor: %f %f %f %f\n", options.averageColor.r, options.averageColor.g, options.averageColor.b, options.averageColor.a);
	fprintf(fp, "averageColorDeviation: %f %f %f %f\n", options.averageColorDeviation.r,
		options.averageColorDeviation.g, options.averageColorDeviation.b, options.averageColorDeviation.a);
	
	fclose(fp);
}

OptionsWindow::~OptionsWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

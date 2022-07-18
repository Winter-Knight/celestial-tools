#include "celestialworlds.hpp"

#include "glprogram.h"
#include "parser.h"
#include "celestial.h"
#include "skypanorama.h"

namespace celestial {

void World::Init()
{
	// OpenGL Shapes
	Celestial::sphere.Init(4);
	Celestial::plane.Init();
}

void World::loadSystemFile(const char * systemFile)
{
	Camera * tmpCamera = getCameraFromFile(systemFile);
	if (tmpCamera) {
		camera = *tmpCamera;
		delete tmpCamera;
	}

	skypanorama = new SkyPanorama();
	skypanorama->Init(getStarboxFromFile(systemFile).c_str());
	skypanorama->program = new GLProgram("shaders/skypanorama.vert", "shaders/skypanorama.frag");

	parseSystemFile(celestials, systemFile);

	// Init light array (array of celestial bodies that emit light)
	for (unsigned int i = 0; i < celestials.size(); i++)
		if (celestials[i]->light.r > 0.0f || celestials[i]->light.g > 0.0f || celestials[i]->light.b > 0.0f)
			Celestial::lightArray.indexList.push_back(i);
}

void World::putCameraOnBody(const char * body)
{
	currentBody = celestialIDMap[body];
}

Celestial * World::getCelestial(const char * celestialName)
{
	return celestials[celestialIDMap[celestialName]];
}

int World::getNumLights()
{
	return Celestial::lightArray.indexList.size();
}

glm::vec4 World::getLightPos(int index)
{
	index = Celestial::lightArray.indexList[index];
	return glm::vec4(celestials[index]->pos, 1.0f);
}

glm::vec4 World::getLightColor(int index)
{
	index = Celestial::lightArray.indexList[index];
	return celestials[index]->light;
}


void World::setTime(long long time)
{
	// Update celestial positions
	for (unsigned int i = 0; i < celestials.size(); i++)
		celestials[i]->Update(time);

	// Update light array (array of celestial bodies that emit light)
	Celestial::lightArray.ClearLights();
	for (unsigned int i = 0; i < Celestial::lightArray.indexList.size(); i++) {
		unsigned int index = Celestial::lightArray.indexList[i];
		Celestial::lightArray.AddLight(glm::vec4(celestials[index]->pos, 1.0f), celestials[index]->light);
	}
}

void World::drawPerspective(const Camera & camera)
{
	Camera drawCam;
	if (currentBody >= 0) {
		drawCam.perspective = this->camera.perspective;
		drawCam.pos = celestials[currentBody]->pos;
		drawCam.dir = camera.dir;
		drawCam.up = camera.up;
		drawCam.CalculateView();
	
		glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(celestials[currentBody]->tilt), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), celestials[currentBody]->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 R3 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 Rotations = R1 * R2 * R3;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), celestials[currentBody]->pos);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f), drawCam.dir, drawCam.up);


		drawCam.view = view * glm::inverse(Rotations) * glm::inverse(T);
	}
	else
		drawCam = camera;

	// Draw all objects
	skypanorama->Draw(&drawCam);
	for (unsigned int i = 0; i < celestials.size(); i++)
		if ((int) i != currentBody)
			celestials[i]->Draw(&drawCam);
}

}

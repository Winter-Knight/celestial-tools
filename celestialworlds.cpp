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

bool World::loadSystemFile(const char * systemFile)
{
	Camera * tmpCamera = getCameraFromFile(systemFile);
	if (tmpCamera) {
		camera = *tmpCamera;
		delete tmpCamera;
	}
	else
		return false;

	skyArray = getSkyArrayFromFile(systemFile);
	parseSystemFile(celestials, systemFile);

	// Init light array (array of celestial bodies that emit light)
	for (unsigned int i = 0; i < celestials.size(); i++)
		if (celestials[i]->light.r > 0.0f || celestials[i]->light.g > 0.0f || celestials[i]->light.b > 0.0f)
			Celestial::lightArray.indexList.push_back(i);
	return true;
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

glm::vec4 World::getLightDir(int index)
{
	glm::vec4 d;
	if (currentBody > -1)
		d = glm::vec4(celestials[currentBody]->pos, 1.0f);
	else
		return glm::vec4(0.0f);

	int indexS = Celestial::lightArray.indexList[index];
	glm::vec4 s = glm::vec4(celestials[index]->pos, 1.0f);

	glm::vec4 ret = glm::inverse(Rotations) * glm::normalize(s - d);
	return ret;
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

		glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(celestials[currentBody]->tilt), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), celestials[currentBody]->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 R3 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Rotations = R1 * R2 * R3;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), celestials[currentBody]->pos);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f), camera.dir, camera.up);

		drawCam.view = view * glm::inverse(T * Rotations);
	}
	else
		drawCam = camera;
	
//	printf("drawCam.view:\n");
//	printMatrix(drawCam.view);

	/* At 100000000000000 us:
		drawCam.view:
		[-0.3978][-0.9175][ 0.0000][66.0101]
		[-0.9034][ 0.3917][ 0.1746][159.6169]
		[-0.1602][ 0.0695][-0.9846][-28.1087]
		[ 0.0000][ 0.0000][ 0.0000][ 1.0000]
	*/

	// Draw all objects
//	skypanorama->Draw(&drawCam, 1.0f);
	if (skyArray) {
		unsigned int lightIndex = skyArray->getLightIndex();
		glm::vec3 lightDir = getLightDir(lightIndex);
		float angle = asin(lightDir.y);
		if (lightDir.x < 0.0f)
			angle = glm::pi<float>() - angle;
		if (angle < 0.0f)
			angle += glm::pi<float>() * 2.0f;
		angle = glm::degrees(angle);
		skyArray->Draw(&drawCam, angle);
	}

	for (unsigned int i = 0; i < celestials.size(); i++)
		if ((int) i != currentBody)
			celestials[i]->Draw(&drawCam);
}

}

#ifndef CELESTIALWORLDS_HPP
#define CELESTIALWORLDS_HPP

#include <vector>

#include <glm/glm.hpp>

#include "camera.h"

class Celestial;
class SkyPanorama;

namespace celestial {

class World {
public:
	void Init();
	void loadSystemFile(const char * systemFile);
	Camera & getCamera() { return camera; }
	void putCameraOnBody(const char * body);
	void setTime(long long time);
	void setAspect(float a) { camera.aspect = a; camera.CalculatePerspective(); }
	void drawPerspective(const Camera & camera);

	Celestial * getCelestial(const char * celestialName);
	int getNumLights();
	glm::vec4 getLightPos(int index);
	glm::vec4 getLightColor(int index);

private:
	Camera camera;
	SkyPanorama * skypanorama;
	std::vector<Celestial *> celestials;
	int currentBody = -1;
};

}

#endif // CELESTIALWORLDS_HPP

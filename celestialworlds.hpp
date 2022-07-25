#ifndef CELESTIALWORLDS_HPP
#define CELESTIALWORLDS_HPP

#include <vector>

#include <glm/glm.hpp>

#include "camera.hpp"

class Celestial;
class SkyArray;

namespace celestial {

class World {
public:
	void Init();
	bool loadSystemFile(const char * systemFile);
	Camera & getCamera() { return camera; }
	void putCameraOnBody(const char * body);
	void setTime(long long time);
	void setAspect(float a) { camera.aspect = a; camera.CalculatePerspective(); }
	void drawPerspective(const Camera & camera);

	Celestial * getCelestial(const char * celestialName);
	int getNumLights();
	glm::vec4 getLightPos(int index);
	glm::vec4 getLightDir(int index);
	glm::vec4 getLightColor(int index);

private:
	Camera camera;
	SkyArray * skyArray;
	std::vector<Celestial *> celestials;
	int currentBody = -1;
	glm::mat4 Rotations;
};

}

#endif // CELESTIALWORLDS_HPP

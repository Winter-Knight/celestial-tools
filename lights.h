#ifndef LIGHTARRAY_H
#define LIGHTARRAY_H

#include <vector>
#include <glm/glm.hpp>

class LightSource {
public:
	glm::vec4 pos;
	glm::vec4 color;
};

class LightArray {
public:
	void AddLight(glm::vec4 pos, glm::vec4 color);
	void ClearLights() { lights.clear(); }

	std::vector<LightSource> lights;
	std::vector<unsigned int> indexList; // List of celestialIDs which contain lights. Used every frame to update lights.
};

#endif // LIGHTARRAY_H

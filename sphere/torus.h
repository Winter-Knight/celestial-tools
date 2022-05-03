#ifndef TORUS_H
#define TORUS_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Torus {
public:
	void InitUV(float radius1, float radius2, int detailU, int detailV);
	void InitSA();
	void Triangulate();
	Torus Double();
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec4> quads;
	std::vector<glm::uvec3> faces;
	
	glm::vec2 radii;
};

#endif // TORUS_H

#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Sphere {
public:

	void InitIcosahedron();
	void InitOctahedron();
	Sphere Quadruple();
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec3> faces;
};

#endif // SPHERE_H

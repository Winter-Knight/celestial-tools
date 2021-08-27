#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <map>
#include <glm/glm.hpp>

class Sphere {
public:

	void Init();
	Sphere Quadruple();
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec3> faces;
	std::map<unsigned long long, unsigned int> edges;
};

#endif // SPHERE_H

#ifndef STAR_H
#define STAR_H

#include <glm/glm.hpp>

#include "camera.h"
#include "sphere.h"

#include "../gl-program.h"

enum StarType {
	SPHERES,
	BILLBOARDS,
	SPHERES_OLD
};

struct Star {
	glm::vec3 pos;
	float size;
	glm::vec4 color;
	static Sphere sphere;
};

class StarArray {
public:
	void AddStar(glm::vec3 pos, float size);
	void AddStar(Star * star);
	void ClearStars();

	void Update(StarType starType);
	void Draw(Camera * camera);
	void SaveToPNG(StarType starType);

//private:
	void MakeSpheres();
	void MakeSpheresFaster();
	void MakeBillboards();

	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;
	GLuint colorBuffer = 0;
	GLuint indexBuffer = 0;

	GLProgram * sphereProgram;
	GLProgram * billboardProgram;

	std::vector<Star> stars;
	GLProgram * activeProgram;
};

#endif // STAR_H

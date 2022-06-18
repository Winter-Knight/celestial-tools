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

	void Init(bool * starsAreBillboards);
	void Update();
	void Draw(Camera * camera);
	void DrawWrapped();

//private:
	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;

	bool * starsAreBillboards;
	GLProgram * drawBillboardProgramPerspective;
	GLProgram * drawBillboardProgramWrap;
	GLProgram * drawSphereProgramPerspective;
	GLProgram * drawSphereProgramWrap;

	std::vector<Star> stars;
};

// Draws a StarArray
class StarArrayDrawerBillboards {
public:
	void Init(StarArray * sa);
	void Update();
	void Draw(Camera * camera);
	void DrawWrapped();

//private:
	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;

	GLProgram * drawProgramPerspective;
	GLProgram * drawProgramWrap;

	StarArray * starArray;
};

#endif // STAR_H

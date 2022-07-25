#ifndef GROUNDPLANE_H
#define GROUNDPLANE_H

#include "camera.hpp"

#include "resource-handler.h"

class GroundPlane {
public:
	void Init();
	void Draw(celestial::Camera * mainCamera);

private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	
	GLProgram * program;
	celestial::Camera camera;
};

#endif // GROUNDPLANE_H

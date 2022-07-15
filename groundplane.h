#ifndef GROUNDPLANE_H
#define GROUNDPLANE_H

#include "camera.h"

#include "glprogram.h"

class GroundPlane {
public:
	void Init();
	void Draw(Camera * mainCamera);

private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	
	GLProgram * program;
	Camera camera;
};

#endif // GROUNDPLANE_H

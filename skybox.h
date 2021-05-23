#ifndef SKYBOX_H
#define SKYBOX_H

#include "../gl-program.h"

#include "camera.h"

class Skybox {
public:

	void Init();
	void Draw(Camera * camera);

	const char * dirName;
	
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint textureBuffer;
	GLProgram * program;
};







#endif // SKYBOX_H

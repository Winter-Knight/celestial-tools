#ifndef SKYPANORAMA_H
#define SKYPANORAMA_H

#include "texture.h"
#include "../gl-program.h"

class SkyPanorama {
public:

	void Init(const char * filename);
	void Draw(Camera * camera);

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	Texture texture;
	GLProgram * program;
};

#endif // SKYPANORAMA_H

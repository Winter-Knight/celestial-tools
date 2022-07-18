#ifndef SKYPANORAMA_H
#define SKYPANORAMA_H

#include "texture.h"
#include "glprogram.h"
#include "camera.h"

class SkyPanorama {
public:

	void Init(const char * filename);
	void Draw(celestial::Camera * camera);

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	Texture texture;
	GLProgram * program;
};

#endif // SKYPANORAMA_H

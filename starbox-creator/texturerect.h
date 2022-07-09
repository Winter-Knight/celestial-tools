#ifndef TEXTURERECT_H
#define TEXTURERECT_H

#include "window.h"
#include "../gl-program.h"

class TextureRect {
public:
	void Init(GLuint tb);
	void Draw();

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	
	GLProgram * program;
};

#endif // TEXTURERECT_H

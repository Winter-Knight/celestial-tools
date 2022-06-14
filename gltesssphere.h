#ifndef GL_TESSSPHERE_H
#define GL_TESSSPHERE_H

#include "sphere.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

class GLTessSphere {
public:
	void Bind(glm::vec3 pos, float size, float tilt);
	
	int numVertices;
	int numIndices;
	
private:
	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;
};

#endif // GL_TESSSPHERE_H

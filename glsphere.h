#ifndef GL_SPHERE_H
#define GL_SPHERE_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "sphere.h"

class GLSphere {
public:
	void Init(int level);
	void Bind();
	
	int numVertices;
	int numIndices;
	
private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
};

#endif // GL_SPHERE_H

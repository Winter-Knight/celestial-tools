#ifndef GL_SPHERE_H
#define GL_SPHERE_H

#ifndef _WIN32
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
#endif // _WIN32

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

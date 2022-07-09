#ifndef GLPLANE_H
#define GLPLANE_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

class GLPlane {
public:
	void Init();
	void Bind();
	
	int numVertices;
	int numIndices;
	
private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
};

#endif // GLPLANE_H

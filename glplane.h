#ifndef GLPLANE_H
#define GLPLANE_H

#ifndef _WIN32
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
#endif // _WIN32

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

#include "glplane.h"

void GLPlane::Init()
{
	numVertices = 4;
	numIndices = 6;
	
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	float vertexdata[] = { -1.0f, -1.0f, 0.0f,
	                        1.0f, -1.0f, 0.0f,
	                        1.0f,  1.0f, 0.0f,
	                       -1.0f,  1.0f, 0.0f };

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexdata), vertexdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	unsigned int indexdata[] = { 0, 1, 3, 1, 2, 3 };
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexdata), indexdata, GL_STATIC_DRAW);
}

void GLPlane::Bind()
{
	glBindVertexArray(vertexArray);
}

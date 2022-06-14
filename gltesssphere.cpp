#include "gltesssphere.h"

void GLTessSphere::Bind(glm::vec3 pos, float size, float tilt)
{
	numVertices = numIndices = 1;

	if (!vertexArray)
		glGenVertexArrays(1, &vertexArray);
	
	if (!vertexBuffer)
		glGenBuffers(1, &vertexBuffer);
	
	float bufferData[] = { pos.x, pos.y, pos.z, size, tilt };
	
	glBindVertexArray(vertexArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(float), bufferData, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) sizeof(glm::vec3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) sizeof(glm::vec4));
}

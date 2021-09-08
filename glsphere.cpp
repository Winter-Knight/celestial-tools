#include "glsphere.h"

void GLSphere::Init(int level)
{
	Sphere sphere1;
	Sphere sphere2;
	if (level % 2 == 0)
		sphere1.InitOctahedron();
	else
		sphere2.InitOctahedron();
	
	while (level) {
		level--;
		if (level % 2 == 0)
			sphere1 = sphere2.Quadruple();
		else
			sphere2 = sphere1.Quadruple();
	}
	
	numVertices = sphere1.vertices.size();
	numIndices = sphere1.faces.size() * 3;
	
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), sphere1.vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), sphere1.faces.data(), GL_STATIC_DRAW);
}

void GLSphere::Bind()
{
	glBindVertexArray(vertexArray);
}

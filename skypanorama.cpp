#include <glm/glm.hpp>

#include "camera.h"
#include "skypanorama.h"
#include "resource-handler.h"

void SkyPanorama::Init(const char * filename)
{
	// Vertex Array

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Vertex Buffers

	glm::vec3 vertices[] = { glm::vec3(-1.0f, -1.0f, 1.0f),
	                         glm::vec3( 0.0f,  1.0f, 0.0f),
	                         glm::vec3( 1.0f, -1.0f, 1.0f),
	                         glm::vec3( 1.0f,  1.0f, 0.0f),
	                         glm::vec3( 1.0f,  1.0f, 1.0f),
	                         glm::vec3( 1.0f,  0.0f, 0.0f),
	                         glm::vec3(-1.0f,  1.0f, 1.0f),
	                         glm::vec3( 0.0f,  0.0f, 0.0f) };

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) sizeof(glm::vec3));

	// Texture
	
	texture = getTexture(filename);
}

void SkyPanorama::Draw(Camera * camera)
{
	glBindVertexArray(vertexArray);
	program->Use();

	glm::vec4 projectionVec(camera->perspective[0][2], camera->perspective[0][0], camera->perspective[1][2], camera->perspective[1][1]);
	program->SetUniform4f("asym_proj", &projectionVec[0]);

	glm::mat4 inverseView = glm::inverse(camera->view);
	program->SetUniformMatrix4f("pano_transform", &inverseView[0][0]);

	texture.Bind(program, 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

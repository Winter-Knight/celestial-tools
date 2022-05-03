#include "skybox.h"
#include "texture.h"

void Skybox::Init()
{
	// Vertex Array

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	

	// Vertex Buffers

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GLfloat vertices[] = { -1.0f, -1.0f, -1.0f,
	                        1.0f, -1.0f, -1.0f,
	                        1.0f,  1.0f, -1.0f,
	                       -1.0f,  1.0f, -1.0f,
	                       -1.0f, -1.0f, 1.0f,
	                        1.0f, -1.0f, 1.0f,
	                        1.0f,  1.0f, 1.0f,
	                       -1.0f,  1.0f, 1.0f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	GLushort indices[] = { 0, 1, 2, // Back
	                       2, 3, 0,
	                       2, 6, 7, // Top
	                       7, 3, 2,
	                       4, 0, 3, // Left
	                       3, 7, 4,
	                       4, 5, 1, // Bottom
	                       1, 0, 4,
	                       1, 5, 6, // Right
	                       6, 2, 1,
	                       4, 6, 5, // Front
	                       6, 4, 7 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	

	// Texture

	const char * filenames[] = { "posx.png", "negx.png", "posy.png", "negy.png",
		"posz.png", "negz.png" };
	
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureBuffer);

	SDL_Surface * image;
	std::string dir = dirName;
	dir += "/";

	for (unsigned int i = 0; i < 6; i++) {
		std::string filename = dir + filenames[i];
		image = LoadImage(filename.c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
		SDL_FreeSurface(image);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Skybox::Draw(Camera * camera)
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	
	program->Use();
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f), camera->dir, camera->up);
	glm::mat4 MVP = camera->perspective * view;
	program->SetUniformMatrix4f("MVP", &MVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureBuffer);
	program->SetUniformi("skyboxTexture", 0);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void *) 0);
}

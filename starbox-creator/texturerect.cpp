#include "texturerect.h"
#include "resource-handler.h"

void TextureRect::Init(GLuint tb)
{
	// Vertex Array
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	float rect[] = { -1.0f, -1.0f,
	                  1.0f, -1.0f,
	                  1.0f,  1.0f,
	                 -1.0f,  1.0f };
	
	// Vertex and Texture Buffers
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	textureBuffer = tb;
	
	// Program
	program = getProgram("shaders/drawRect.vert", "shaders/drawRect.frag");
	program->Use();
	program->SetUniformi("texture0", 0);
}

void TextureRect::Draw()
{
	program->Use();
	
	glBindVertexArray(vertexArray);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

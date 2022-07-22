#ifndef TERRAIN_H
#define TERRAIN_H

#include "camera.h"
#include "glprogram.h"
#include "texture.h"

class Terrain {
public:
	void Init();
	void Draw(const celestial::Camera & camera);
	float getHeight(int x, int y);
	float getHeight(glm::vec2 pos);
	void setLight(glm::vec4 dir, glm::vec4 color) { lightDir = dir; lightColor = color; }

	const static int terrainWidth = 512;

private:
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	Texture rockyTexture;
	Texture normalTexture;
	GLProgram * program;

	std::vector<glm::vec3> vertexdata;
	
	glm::vec4 lightDir;
	glm::vec4 lightColor;
};

#endif // TERRAIN_H

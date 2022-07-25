#include <vector>
//#include <FastNoise/FastNoise.h>

#include "terrain.h"
#include "resource-handler.h"
#include "texture.h"

#ifndef RESOURCE_BASE_DIR
	#define RESOURCE_BASE_DIR ""
#endif

const char * heightTextureFilename = RESOURCE_BASE_DIR "images/terrainheightmap.png";
const char * normalTextureFilename = RESOURCE_BASE_DIR "images/terrainnormalmap.png";
const char * rockyTextureFilename = RESOURCE_BASE_DIR "images/rock_04_diff_2k.jpg";

const float HEIGHT_MULTIPLIER = 400.0f;

float Terrain::getHeight(int x, int y)
{
	x = glm::clamp(x, 0, terrainWidth);
	y = glm::clamp(y, 0, terrainWidth);

	return vertexdata[y * (terrainWidth + 1) + x].y;
}

float Terrain::getHeight(glm::vec2 pos)
{
	int x0 = glm::floor(pos.x);
	int y0 = glm::floor(pos.y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	
	float xlerp = glm::fract(pos.x);
	float ylerp = glm::fract(pos.y);
	
	float height00 = getHeight(x0, y0);
	float height10 = getHeight(x1, y0);
	float height01 = getHeight(x0, y1);
	float height11 = getHeight(x1, y1);

	float height0a = glm::mix(height00, height10, xlerp);
	float height0b = glm::mix(height01, height11, xlerp);
	return glm::mix(height0a, height0b, ylerp);
}

void Terrain::Init()
{
	// Vertex Array
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	// Vertex Buffer
	vertexdata.clear();

/*	auto perlin = FastNoise::New<FastNoise::Perlin>();
	auto fractal = FastNoise::New<FastNoise::FractalRidged>();
	fractal->SetSource(perlin);
	fractal->SetOctaveCount(5);

	std::vector<float> heightdata((terrainWidth) * (terrainWidth));
	fractal->GenTileable2D(heightdata.data(), terrainWidth, terrainWidth, 0.003f, 1);
*/

	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(heightTextureFilename);
	FIBITMAP * heightImage = FreeImage_Load(fif, heightTextureFilename, 0);
	if (!heightImage) {
		printf("Error: Image not found: %s!\n", heightTextureFilename);
		return;
	}

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(heightImage);
	printf("image filename: %s     fif: %d     format: %d\n", heightTextureFilename, fif, imageType);
	if (imageType != FIT_UINT16)
		printf("WARNING: Image type for %s may not be supported. Image type: %d\n", heightTextureFilename, imageType);
	if (FreeImage_GetBPP(heightImage) != 16)
		printf("Was expecting 16 bpp. bpp: %d\n", FreeImage_GetBPP(heightImage));

	unsigned short * heightdata = (unsigned short *) FreeImage_GetBits(heightImage);

	for (int j = 0; j < terrainWidth; j++) {
		for (int i = 0; i < terrainWidth; i++) {
//			float height = heightdata[j * (terrainWidth) + i] * 200.0f;
			float height = float(heightdata[j * (terrainWidth) + i]) / float(USHRT_MAX) * HEIGHT_MULTIPLIER;
			glm::vec3 pos = glm::vec3(i, height, j);
			vertexdata.push_back(pos);

			if (i % terrainWidth == (terrainWidth - 1)) {
//				height = heightdata[j * terrainWidth] * 200.0f;
				height = float(heightdata[j * terrainWidth]) / float(USHRT_MAX) * HEIGHT_MULTIPLIER;
				pos = glm::vec3(terrainWidth, height, j);
				vertexdata.push_back(pos);
			}
		}
	}
	// Last row
	for (int i = 0; i < terrainWidth; i++) {
//		float height = heightdata[i] * 200.0f;
		float height = float(heightdata[i]) / float(USHRT_MAX) * HEIGHT_MULTIPLIER;
		glm::vec3 pos = glm::vec3(i, height, terrainWidth);
		vertexdata.push_back(pos);
	}
	// Last point in far corner
//	float height = heightdata[0] * 200.0f;
	float height = float(heightdata[0]) / float(USHRT_MAX) * HEIGHT_MULTIPLIER;
	glm::vec3 pos = glm::vec3(terrainWidth, height, terrainWidth);
	vertexdata.push_back(pos);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexdata.size() * sizeof(glm::vec3), vertexdata.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	// Index Buffer
	std::vector<unsigned int> indexdata;
	for (unsigned int j = 0; j < terrainWidth; j++) {
		for (unsigned int i = 0; i < terrainWidth; i++) {
			indexdata.push_back((j + 0) * (terrainWidth + 1) + i + 0);
			indexdata.push_back((j + 1) * (terrainWidth + 1) + i + 0);
			indexdata.push_back((j + 0) * (terrainWidth + 1) + i + 1);
			indexdata.push_back((j + 1) * (terrainWidth + 1) + i + 0);
			indexdata.push_back((j + 1) * (terrainWidth + 1) + i + 1);
			indexdata.push_back((j + 0) * (terrainWidth + 1) + i + 1);
		}
	}
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexdata.size() * sizeof(unsigned int), indexdata.data(), GL_STATIC_DRAW);

	// Texture
	rockyTexture = getTexture(rockyTextureFilename);
	rockyTexture.Enhance();

	normalTexture = getTexture(normalTextureFilename);
	normalTexture.Enhance();

	// Program
	program = getProgram("shaders/terrain.vert", "shaders/terrain.frag");
}

void Terrain::Draw(const celestial::Camera & camera)
{
	program->Use();
	glm::mat4 VP = camera.perspective * camera.view;
	program->SetUniformMatrix4f("VP", &VP[0][0]);
	program->SetUniform4f("lightDir", &lightDir[0]);
	program->SetUniform4f("lightColor", &lightColor[0]);
	rockyTexture.Bind(program, 0);
	normalTexture.Bind(program, 1);
	glBindVertexArray(vertexArray);
	
	glDrawElementsInstanced(GL_TRIANGLES, terrainWidth * terrainWidth * 6, GL_UNSIGNED_INT, 0, 9);
}

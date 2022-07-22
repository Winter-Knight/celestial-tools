#include <glm/glm.hpp>

#include "sdl/window.h"

#include "camera.h"
#include "skypanorama.h"
#include "resource-handler.h"

using namespace celestial;

SkyPanorama::SkyPanorama()
{
	if (!program)
		program = getProgram("shaders/skypanorama.vert", "shaders/skypanorama.frag");
}

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
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) sizeof(glm::vec3));

	// Texture
	
	texture = getTexture(filename);
	texture.Enhance();
}

void SkyPanorama::Draw(celestial::Camera * camera, float alpha)
{
	glBindVertexArray(vertexArray);
	program->Use();

	glm::vec4 projectionVec(camera->perspective[0][2], camera->perspective[0][0], camera->perspective[1][2], camera->perspective[1][1]);
	program->SetUniform4f("asym_proj", &projectionVec[0]);

	glm::mat4 inverseView = glm::inverse(camera->view);
	program->SetUniformMatrix4f("pano_transform", &inverseView[0][0]);

	program->SetUniformf("alpha", alpha);

	texture.Bind(program, 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

SkyColor::SkyColor()
{
	if (!program)
		program = getProgram("shaders/skypanorama.vert", "shaders/skycolor.frag");
}

void SkyColor::Init(const glm::vec3 & c)
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
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) sizeof(glm::vec3));

	// Color
	
	color = c;
}

void SkyColor::Draw(celestial::Camera * camera, float alpha)
{
	glBindVertexArray(vertexArray);
	program->Use();
	program->SetUniform3f("skyColor", &color[0]);
	program->SetUniformf("alpha", alpha);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void SkyArray::Draw(celestial::Camera * camera, float angle)
{
	if (skymap.empty())
		return;
	
	float currentAngle;
	float nextAngle;
	for (auto iter = skymap.begin();;) {
		currentAngle = nextAngle = iter->first;
		iter++;
		if (iter == skymap.end()) {
			nextAngle = skymap.begin()->first;
			break;
		}
		else {
			nextAngle = iter->first;
		}
		
		if (currentAngle < angle && angle < nextAngle)
			break;
	}
	
	float effectiveNextAngleValue = nextAngle;
	if (currentAngle > effectiveNextAngleValue)
		effectiveNextAngleValue += 360.0f;
	
	float angleProgression = angle - currentAngle;
	if (angleProgression < 0.0f)
		angleProgression += 360.0f;
	float angleDifference = effectiveNextAngleValue - currentAngle;
	
	float alpha;
	if (skymap[nextAngle] != skymap[currentAngle])
		alpha = glm::mix(1.0f, 0.0f, angleProgression / angleDifference);
	else
		alpha = 1.0f;
	
	skymap[currentAngle]->Draw(camera, alpha);
	if (alpha != 1.0f) {
		alpha = glm::mix(0.0f, 1.0f, angleProgression / angleDifference);
		skymap[nextAngle]->Draw(camera, alpha);
	}
//	printf("%f     %f     %f     %f\n", currentAngle, nextAngle, angle, alpha);	
}

void SkyArray::printSkymap()
{
	// TODO
}

GLProgram * SkyPanorama::program = NULL;
GLProgram * SkyColor::program = NULL;

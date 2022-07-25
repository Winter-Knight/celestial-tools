/*************************************************************************/
/* Copyright (c) 2022 Brandon Barnes <winterknight@email.com>            */
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/


#include <glm/glm.hpp>

#include "camera.hpp"
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

	float vertices[] = { -1.0f, -1.0f,
	                      1.0f, -1.0f,
	                      1.0f,  1.0f,
	                     -1.0f,  1.0f };
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

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

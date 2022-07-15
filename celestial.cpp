#include <sstream>

#include "celestial.h"
#include "resource-handler.h"

void Orbital::Update(InputHandler * input, long long time)
{
	if (input->paused)
		return;

	if (!semi_major_axis)
		semi_major_axis = distance;

	long long orbitTime, rotationTime;
	orbitTime = rotationTime = time;

	orbitTime = orbitTime % period;

	float angle_in_radians = float(orbitTime + period_offset) / period * 2 * glm::pi<float>();
	float semi_minor_axis = semi_major_axis * sqrt(1 - eccentricity * eccentricity);

	float b2 = semi_minor_axis * semi_minor_axis;
	float a2 = semi_major_axis * semi_major_axis;
	float sinAngle = sin(angle_in_radians);
	float cosAngle = cos(angle_in_radians);
	float tan2 = sinAngle / cosAngle; tan2 = tan2 * tan2;
	pos.x = sqrt(b2 / (tan2 + b2 / a2));
	if (cosAngle < 0.0f)
		pos.x *= -1.0f;
	pos.z = -sqrt(a2 / ((1 / tan2) + a2 / b2));
	if (sinAngle < 0.0f)
		pos.z *= -1.0f;
	pos += orbitObject->pos;
	
	float focus_center_distance = sqrt(a2 - b2);
	pos.x += focus_center_distance;

	
	if (rotation_period) {
		rotationTime = rotationTime % rotation_period;
		rotation = float(rotationTime) / rotation_period * 2 * glm::pi<float>();
	}
}

void Celestial::AddTexture(const char * filename)
{
	Texture texture;
	texture = getTexture(filename);
	textures.push_back(texture);
}

// CPU Sphere
void Celestial::Draw(Camera * camera)
{
	program->Use();

	glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(tilt), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rotations = R1 * R2;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 VP = camera->perspective * camera->view;

	program->SetUniformMatrix4f("VP", &VP[0][0]);
	program->SetUniformMatrix4f("Rotations", &Rotations[0][0]);
	program->SetUniformMatrix4f("Translation", &T[0][0]);

	program->SetUniform3f("centerPos_worldspace", &pos[0]);
	program->SetUniformf("size", size);

	std::stringstream ss;

	for (unsigned int i = 0; i < lightArray.lights.size(); i++) {
		ss.str("");
		ss << "lights[" << i << "].pos";
		program->SetUniform4f(ss.str().c_str(), &lightArray.lights[i].pos[0]);

		ss.str("");
		ss << "lights[" << i << "].color";
		program->SetUniform4f(ss.str().c_str(), &lightArray.lights[i].color[0]);
	}

	for (unsigned int i = 0; i < textures.size(); i++)
		textures[i].Bind(program, i);

	if (shape == SPHERE) {
		sphere.Bind();
		glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, 0);
	}
	else if (shape == PLANE) {
		plane.Bind();
		glDrawElements(GL_TRIANGLES, plane.numIndices, GL_UNSIGNED_INT, 0);
	}
}

void Celestial::Draw(Camera * camera, SphereType sphereType)
{
	if (sphereType == CPUSPHERE)
		Draw(camera);
	else if (sphereType == TESSSPHERE) {
		
		program->Use();
		glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(tilt), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 Rotations = R1 * R2;
		glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 VP = camera->perspective * camera->view;
	
		program->SetUniformMatrix4f("VP", &VP[0][0]);
		program->SetUniformMatrix4f("Rotations", &Rotations[0][0]);
		program->SetUniformMatrix4f("Translation", &T[0][0]);
		
		tessSphere.Bind(pos, size, tilt);
		
		for (unsigned int i = 0; i < textures.size(); i++)
			textures[i].Bind(program, i);
		
		glDrawArrays(GL_PATCHES, 0, 1);
	}
}

GLSphere Celestial::sphere;
GLPlane Celestial::plane;
LightArray Celestial::lightArray;

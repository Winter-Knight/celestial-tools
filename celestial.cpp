#include "celestial.h"

#include "../gl-debug.h"

void Celestial::Init()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	GLuint sizeBuffer;
	glGenBuffers(1, &sizeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), &size, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void *) 0);
}

void Orbital::Update(InputHandler * input, float time)
{
	if (input->paused)
		return;

	if (!semi_major_axis)
		semi_major_axis = distance;
	
	float angle_in_radians = (time + period_offset) / period * 2 * glm::pi<float>();
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

	
//	printf("pos: [%8.2f][%8.2f][%8.2f],   semi_major_axis: %f,   tan2: %f, tan(a): %f\n", pos.x, pos.y, pos.z, semi_major_axis, tan2, tan(angle_in_radians));

//	pos.x = orbitObject->pos.x + cos(angle_in_radians) * distance;
//	pos.z = orbitObject->pos.z - sin(angle_in_radians) * distance;
//	printf("Orbital location: [%5.2f][%5.2f][%5.2f]\n", pos.x, pos.y, pos.z);
	if (rotation_period)
		rotation = time / rotation_period * 2 * glm::pi<float>();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &pos, GL_STATIC_DRAW);
}

void Celestial::AddTexture(const char * filename)
{
	Texture texture;
	texture.Init(filename);
	textures.push_back(texture);
}

void Celestial::Draw(Camera * camera)
{
	glBindVertexArray(vertexArray);
	
	program->Use();
	glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(tilt), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Rotations = R1 * R2;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 VP = camera->perspective * camera->view;

	program->SetUniformMatrix4f("VP", &VP[0][0]);
	program->SetUniformMatrix4f("Rotations", &Rotations[0][0]);
	program->SetUniformMatrix4f("Translation", &T[0][0]);

	for (unsigned int i = 0; i < textures.size(); i++)
		textures[i].Bind(program, i);

	glDrawArrays(GL_PATCHES, 0, 1);
}

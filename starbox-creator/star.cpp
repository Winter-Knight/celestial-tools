#include "star.h"

void StarArray::AddStar(glm::vec3 pos, float size)
{
	Star star;
	star.pos = pos;
	star.size = size;
	stars.push_back(star);
}

void StarArray::AddStar(Star * star)
{
	stars.push_back(*star);
}

void StarArray::ClearStars()
{
	stars.clear();
}

void StarArray::Init(bool * sab)
{
	if (!vertexArray) {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		glEnableVertexAttribArray(0); // pos
		glEnableVertexAttribArray(1); // size
		glEnableVertexAttribArray(2); // color
	}
	
	if (!vertexBuffer)
		glGenBuffers(1, &vertexBuffer);

	starsAreBillboards = sab;
}

void StarArray::Draw(Camera * camera)
{
	GLProgram * activeProgram;
	if (*starsAreBillboards)
		activeProgram = drawBillboardProgramPerspective;
	else
		activeProgram = drawSphereProgramPerspective;

	activeProgram->Use();
	
	glm::mat4 VP = camera->perspective * camera->view;
	activeProgram->SetUniformMatrix4f("VP", &VP[0][0]);

	glm::vec3 cameraRight(camera->view[0][0], camera->view[1][0], camera->view[2][0]);
	glm::vec3 cameraUp(camera->view[0][1], camera->view[1][1], camera->view[2][1]);
	activeProgram->SetUniform3f("cameraRight", &cameraRight[0]);
	activeProgram->SetUniform3f("cameraUp", &cameraUp[0]);

	GLint location;
	location = glGetUniformLocation(activeProgram->getProgramID(), "vertices");
	glUniform3fv(location, Star::sphere.vertices.size(), &Star::sphere.vertices[0][0]);
	location = glGetUniformLocation(activeProgram->getProgramID(), "indices");
	glUniform1uiv(location, Star::sphere.faces.size() * 3, &Star::sphere.faces[0][0]);

	glDrawArrays(GL_POINTS, 0, stars.size());
}

void StarArray::DrawWrapped()
{
	if (*starsAreBillboards)
		drawBillboardProgramWrap->Use();
	else
		drawSphereProgramWrap->Use();
	glDrawArrays(GL_POINTS, 0, stars.size());
}

void StarArray::Update()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(Star), &stars[0], GL_STATIC_DRAW);

	// Star Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, pos));
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, size));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, color));
}

Sphere Star::sphere;

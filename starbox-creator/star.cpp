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

void StarArrayDrawerBillboards::Init(StarArray * sa)
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
	
	starArray = sa;
}

void StarArrayDrawerBillboards::Update()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, starArray->stars.size() * sizeof(Star), &starArray->stars[0], GL_STATIC_DRAW);

	// Star Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, pos));
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, size));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, color));
}

void StarArrayDrawerBillboards::Draw(Camera * camera)
{
	glBindVertexArray(vertexArray);
	drawProgramPerspective->Use();
	
	glm::mat4 VP = camera->perspective * camera->view;
	drawProgramPerspective->SetUniformMatrix4f("VP", &VP[0][0]);

	glm::vec3 cameraRight(camera->view[0][0], camera->view[1][0], camera->view[2][0]);
	glm::vec3 cameraUp(camera->view[0][1], camera->view[1][1], camera->view[2][1]);
	drawProgramPerspective->SetUniform3f("cameraRight", &cameraRight[0]);
	drawProgramPerspective->SetUniform3f("cameraUp", &cameraUp[0]);

	glDrawArrays(GL_POINTS, 0, starArray->stars.size());
}

void StarArrayDrawerBillboards::DrawWrapped()
{
	glBindVertexArray(vertexArray);
	drawProgramWrap->Use();
	glDrawArrays(GL_POINTS, 0, starArray->stars.size());
}

void StarArrayDrawerSpheres::Init(StarArray * sa)
{
	if (!vertexArray) {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		glEnableVertexAttribArray(0); // sphere local vecs
		glEnableVertexAttribArray(1); // pos
		glEnableVertexAttribArray(2); // size
		glEnableVertexAttribArray(3); // color
	}

	if (!vertexBuffer)
		glGenBuffers(1, &vertexBuffer);
	
	if (!indexBuffer)
		glGenBuffers(1, &indexBuffer);
	
	starArray = sa;
}

void StarArrayDrawerSpheres::Update()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, starArray->stars.size() * sizeof(Star) + Star::sphere.vertices.size() * sizeof(glm::vec3),
		NULL, GL_STATIC_DRAW);

	// Star Array
	glBufferSubData(GL_ARRAY_BUFFER, 0, starArray->stars.size() * sizeof(Star), &starArray->stars[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, pos));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, size));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Star), (void *) offsetof(Star, color));

	// Sphere Array
	glBufferSubData(GL_ARRAY_BUFFER, starArray->stars.size() * sizeof(Star),
		Star::sphere.vertices.size() * sizeof(glm::vec3), &Star::sphere.vertices[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) (starArray->stars.size() * sizeof(Star)));

	// Index Array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Star::sphere.faces.size() * sizeof(glm::uvec3),
		&Star::sphere.faces[0], GL_STATIC_DRAW);
}

void StarArrayDrawerSpheres::Draw(Camera * camera)
{
	drawProgramPerspective->Use();
	glBindVertexArray(vertexArray);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glm::mat4 VP = camera->perspective * camera->view;
	drawProgramPerspective->SetUniformMatrix4f("VP", &VP[0][0]);

	glDrawElementsInstanced(GL_TRIANGLES, Star::sphere.faces.size() * 3, GL_UNSIGNED_INT, 0, starArray->stars.size());
}

void StarArrayDrawerSpheres::DrawWrapped()
{
	drawProgramWrap->Use();
	glBindVertexArray(vertexArray);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
	glVertexAttribDivisor(3, 0);

	GLint location;
	location = glGetUniformLocation(drawProgramWrap->getProgramID(), "vertices");
	glUniform3fv(location, Star::sphere.vertices.size(), &Star::sphere.vertices[0][0]);
	location = glGetUniformLocation(drawProgramWrap->getProgramID(), "indices");
	glUniform1uiv(location, Star::sphere.faces.size() * 3, &Star::sphere.faces[0][0]);

	glDrawArrays(GL_POINTS, 0, starArray->stars.size());
}

Sphere Star::sphere;

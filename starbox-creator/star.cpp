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

void StarArray::MakeSpheres()
{
	// Vertices
	unsigned int sizeofStarData = stars.size() * Star::sphere.vertices.size() * sizeof(glm::vec3);
	glm::vec3 * stardata = (glm::vec3 *) malloc(sizeofStarData);

	for (unsigned int i = 0; i < stars.size(); i++) {
		for (unsigned int j = 0; j < Star::sphere.vertices.size(); j++) {
			stardata[i * Star::sphere.vertices.size() + j] = stars[i].pos + Star::sphere.vertices[j] * stars[i].size;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeofStarData, stardata, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glDisableVertexAttribArray(2); // Billboards only


	// Indices
	unsigned int sizeofIndexData = stars.size() * Star::sphere.faces.size() * sizeof(glm::uvec3);
	unsigned int * indexdata = (unsigned int *) malloc(sizeofIndexData);
	
	for (unsigned int i = 0; i < stars.size(); i++) {
		for (unsigned int j = 0; j < Star::sphere.faces.size(); j++) {
			indexdata[(i * Star::sphere.faces.size() + j) * 3 + 0] = i * Star::sphere.vertices.size() + Star::sphere.faces[j].x;
			indexdata[(i * Star::sphere.faces.size() + j) * 3 + 1] = i * Star::sphere.vertices.size() + Star::sphere.faces[j].y;
			indexdata[(i * Star::sphere.faces.size() + j) * 3 + 2] = i * Star::sphere.vertices.size() + Star::sphere.faces[j].z;
		}
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndexData, indexdata, GL_STATIC_DRAW);

	free(stardata);
	free(indexdata);
	
	activeProgram = sphereProgram;
}

void StarArray::MakeBillboards()
{
	// Vertices
	unsigned int sizeofStarData = stars.size() * 4 * sizeof(glm::vec3) * 2;
	glm::vec3 * stardata = (glm::vec3 *) malloc(sizeofStarData);
	
	for (unsigned int i = 0; i < stars.size(); i++) {
		for (unsigned int j = 0; j < 4; j++) {
			float x, y;
			x = j % 2 == 0 ? -1.0f : 1.0f;
			y = j < 2 ? -1.0f : 1.0f;

			stardata[i * 8 + j * 2 + 0] = glm::vec3(x, y, stars[i].size);
			stardata[i * 8 + j * 2 + 1] = stars[i].pos;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeofStarData, stardata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void *) sizeof(glm::vec3));

	// Indices
	unsigned int sizeofIndexData = stars.size() * 6 * sizeof(unsigned int);
	unsigned int * indexdata = (unsigned int *) malloc(sizeofIndexData);
	
	for (unsigned int i = 0; i < stars.size(); i++) {
		indexdata[i * 6 + 0] = i * 4 + 0;
		indexdata[i * 6 + 1] = i * 4 + 1;
		indexdata[i * 6 + 2] = i * 4 + 2;
		indexdata[i * 6 + 3] = i * 4 + 2;
		indexdata[i * 6 + 4] = i * 4 + 1;
		indexdata[i * 6 + 5] = i * 4 + 3;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndexData, indexdata, GL_STATIC_DRAW);

	free(stardata);
	free(indexdata);

	activeProgram = billboardProgram;
}

void StarArray::Update(StarType starType)
{
	if (vertexArray && vertexBuffer && colorBuffer && indexBuffer) { // They should have all been initialized together
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &colorBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}
	
	// Vertex Array
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(0); // pos
	
	// Vertex Buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// Index Buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (starType == SPHERES)
		MakeSpheres();
	else if (starType == BILLBOARDS)
		MakeBillboards();

	// Color
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	
	unsigned int colorbuffersize;
	if (starType == BILLBOARDS)
		colorbuffersize = stars.size() * 4 * sizeof(glm::vec4);
	if (starType == SPHERES)
		colorbuffersize = stars.size() * Star::sphere.vertices.size() * sizeof(glm::vec4);

	glm::vec4 * colorbufferdata = (glm::vec4 *) malloc(colorbuffersize);

	for (unsigned int i = 0; i < stars.size(); i++) {
		if (starType == BILLBOARDS)
			for (unsigned int j = 0; j < 4; j++)
				colorbufferdata[i * 4 + j] = stars[i].color;
		if (starType == SPHERES)
			for (unsigned int j = 0; j < Star::sphere.vertices.size(); j++)
				colorbufferdata[i * Star::sphere.vertices.size() + j] = stars[i].color;
	}
	glBufferData(GL_ARRAY_BUFFER, colorbuffersize, colorbufferdata, GL_STATIC_DRAW);	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);
}

void StarArray::Draw(Camera * camera)
{
	unsigned int numIndices;
	
	activeProgram->Use();
	glm::mat4 VP = camera->perspective * camera->view;
	activeProgram->SetUniformMatrix4f("VP", &VP[0][0]);

	if (activeProgram == billboardProgram) {
		glm::vec3 cameraRight(camera->view[0][0], camera->view[1][0], camera->view[2][0]);
		glm::vec3 cameraUp(camera->view[0][1], camera->view[1][1], camera->view[2][1]);
		activeProgram->SetUniform3f("cameraRight", &cameraRight[0]);
		activeProgram->SetUniform3f("cameraUp", &cameraUp[0]);
		
		numIndices = stars.size() * 6;
	}
	else
		numIndices = stars.size() * Star::sphere.faces.size() * 3;

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void StarArray::SaveToPNG(StarType starType)
{
	if (starType != BILLBOARDS) {
		printf("Only billboard savings supported for now\n");
		return;
	}

	static GLProgram * saveStarboxProgram = NULL;
	if (!saveStarboxProgram)
		saveStarboxProgram = new GLProgram("shaders/billboards-wrap.vert", "shaders/billboards.frag");

	saveStarboxProgram->Use();

	glDrawElements(GL_TRIANGLES, stars.size() * 6, GL_UNSIGNED_INT, 0);
}

Sphere Star::sphere;

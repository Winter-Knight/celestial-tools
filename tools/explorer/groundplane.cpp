#include "groundplane.h"

using namespace celestial;

void GroundPlane::Init()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	float vertexdata[] = { -1.0f,  1.0f,
	                        1.0f,  1.0f,
	                        1.0f, -1.0f,
	                       -1.0f, -1.0f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexdata), vertexdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	
	program = getProgram("shaders/groundplane.vert", "shaders/groundplane.frag");
	
	camera.pos = glm::vec3(0.0f, 0.0f, 2.0f);
	
	camera.fov = glm::radians(60.0f);
	camera.aspect = 1;
	camera.near = 0.1f;
	camera.far = 10000.0f;
	
	camera.CalculatePerspective();
}

void GroundPlane::Draw(Camera * mainCamera)
{
	program->Use();
	glBindVertexArray(vertexArray);

	camera.dir = mainCamera->dir;
	camera.up = mainCamera->up;
	camera.CalculateView();
	
	glm::mat4 VP = camera.perspective * camera.view;
	program->SetUniformMatrix4f("VP", &VP[0][0]);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

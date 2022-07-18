#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace celestial {

class Camera {
public:
	void CalculateView() { view = glm::lookAt(pos, pos + dir, up); }
	void CalculatePerspective() { perspective = glm::perspective(fov, aspect, near, far); }

	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 up;
	
	float fov; // in radians
	float aspect;
	float near;
	float far;

	glm::mat4 view;
	glm::mat4 perspective;
};

}

#endif // CAMERA_H

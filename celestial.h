#ifndef CELESTIAL_H
#define CELESTIAL_H

#include <vector>
#include <glm/glm.hpp>

#include "camera.h"
#include "texture.h"
#include "input.h"
#include "glsphere.h"

#include "../gl-program.h"

class Celestial {
public:
	virtual void Update(InputHandler * input, float time) {}
	virtual void Draw(Camera * camera);
	virtual void AddTexture(const char * filename);

// protected:
	glm::vec3 pos;
	float size;
	float rotation = 0.0f; // In radians
	float rotation_period = 0.0f; // In hours
	float tilt = 0.0f; // In degrees

	std::vector<Texture> textures;
	GLProgram * program;
	
	static GLSphere sphere;
};


class Orbital : public Celestial {
public:
	virtual void Update(InputHandler * input, float time);

// protected:
	Celestial * orbitObject;
	float distance;
	float semi_major_axis;
	float eccentricity = 0.0f;
//	float eccentric_rotation = 0.0f; // Should be between 0 and 360
//	int focus = 0; // Should be 0 or 1
	float period; // In hours
	float period_offset = 0.0f; // In hours
};

#endif // CELESTIAL_H

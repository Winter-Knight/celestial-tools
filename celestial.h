#ifndef CELESTIAL_H
#define CELESTIAL_H

#include <vector>
#include <glm/glm.hpp>

#include "camera.h"
#include "texture.h"
#include "input.h"
#include "glsphere.h"
#include "gltesssphere.h"
#include "glplane.h"
#include "lights.h"

#include "glprogram.h"

enum SphereType {
	CPUSPHERE,
	TESSSPHERE,
};

enum Shape {
	SPHERE,
	PLANE
};

class Celestial {
public:
	virtual void Update(InputHandler * input, long long time) {}
	virtual void Draw(Camera * camera);
	virtual void Draw(Camera * camera, SphereType sphereType);
	virtual void AddTexture(const char * filename);

// protected:
	glm::vec3 pos;
	float size;
	float rotation = 0.0f; // In radians
	long long rotation_period = 0; // In microseconds
	float tilt = 0.0f; // In degrees
	Shape shape = SPHERE;
	glm::vec4 light = glm::vec4(0.0f);

	std::vector<Texture> textures;
	GLProgram * program;

	static GLSphere sphere;
	GLTessSphere tessSphere;
	static GLPlane plane;
	static LightArray lightArray;
};


class Orbital : public Celestial {
public:
	virtual void Update(InputHandler * input, long long time);

// protected:
	Celestial * orbitObject;
	float distance;
	float semi_major_axis;
	float eccentricity = 0.0f;
//	float eccentric_rotation = 0.0f; // Should be between 0 and 360
//	int focus = 0; // Should be 0 or 1
	long long period; // In microseconds
	long long period_offset = 0; // In microseconds
};

#endif // CELESTIAL_H

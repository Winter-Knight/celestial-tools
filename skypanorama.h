#ifndef SKYPANORAMA_H
#define SKYPANORAMA_H

#include <map>

#include "texture.h"
#include "glprogram.h"
#include "camera.hpp"

class Sky {
public:
	virtual void Init(const glm::vec3 & color) { printf("Error! Should not be here! Sky::Init(color) Likely a SkyPanorama with a color!\n"); }
	virtual void Init(const char * filename) { printf("Error! Should not be here! Sky::Init(filename) Likely a SkyColor with a texture!\n"); }
	virtual void Draw(celestial::Camera * camera, float alpha) = 0;

protected:
	GLuint vertexArray;
	GLuint vertexBuffer;
};

class SkyPanorama : public Sky {
public:
	SkyPanorama();
	virtual void Init(const char * filename);
	virtual void Draw(celestial::Camera * camera, float alpha);

private:
	Texture texture;
	static GLProgram * program;
};

class SkyColor : public Sky {
public:
	SkyColor();
	virtual void Init(const glm::vec3 & color);
	virtual void Draw(celestial::Camera * camera, float alpha);

private:
	glm::vec3 color;
	static GLProgram * program;
};

class SkyArray {
public:
	void AddSky(Sky * sky, float angle) { skymap[angle] = sky; }
	void ClearSkies() { skymap.clear(); }
	void Draw(celestial::Camera * camera, float angle);

	void setBackgroundColor(glm::vec4 bgc) { backgroundColor = bgc; }
	glm::vec4 getBackgroundColor() { return backgroundColor; }
	void setLightIndex(unsigned int i) { lightIndex = i; }
	unsigned int getLightIndex() { return lightIndex; }

	unsigned int getNumSkies() { return skymap.size(); }
	void printSkymap();

private:
	std::map<float, Sky *> skymap;
	glm::vec4 backgroundColor;
	unsigned int lightIndex; // The index of which light's position determines which skies to draw.
};

#endif // SKYPANORAMA_H

#include <fstream>
#include <sstream>

#include "parser.h"
#include "resource-handler.h"

std::map<std::string, int> celestialIDMap;

using namespace celestial;

void parseSystemFile(std::vector<Celestial *> & celestials, const char * filename, const char * vertexshaderfile)
{
	std::string line, fieldName;
	std::stringstream ss;
	Celestial * celestial = NULL;
	
	std::string strValue;
	float fValue;
	glm::vec3 floats;
	int iValue;
	
	// Open file
	std::ifstream file(filename);
	if (!file.is_open()) {
		printf("Error: Failed to open solar system file: %s! Exiting parse function!\n", filename);
		return;
	}
	
	Texture::assignBaseDir(filename);

	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;
	
		// If line is recognized celestial type, then create new celestial. If not recognized then continue to next line
		if (line == "[Celestial]") {
			celestial = new Celestial();
			celestials.push_back(celestial);
			continue;
		}
		else if (line == "[Orbital]") {
			celestial = new Orbital();
			celestials.push_back(celestial);
			continue;
		}
		// New object that is not recognized by this function
		else if (line[0] == '[')
			celestial = NULL;
		
		if (!celestial)
			continue;

		// Get next word from line, which represents name of field. If field is not reconized continue to next line
		// Get next word from line, which represents value of field.
		ss.clear();
		ss.str(line);
		ss >> fieldName;
		if (fieldName == "Name:") {
			ss >> strValue;
			celestialIDMap[strValue] = celestials.size() - 1;
		}
		if (fieldName == "Position:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			celestial->pos = floats;
		}
		else if (fieldName == "Size:") {
			ss >> fValue;
			celestial->size = fValue;
		}
		else if (fieldName == "Shader:") {
			ss >> strValue;
			celestial->program = getProgram(vertexshaderfile, (Texture::baseDir + strValue).c_str(), false);
		}
		else if (fieldName == "Distance:") {
			ss >> fValue;
			((Orbital *) celestial)->distance = fValue;
		}
		else if (fieldName == "Period:") {
			ss >> fValue;
			((Orbital *) celestial)->period = fValue * 60 * 60 * 1000000;
		}
		else if (fieldName == "Tilt:") {
			ss >> fValue;
			celestial->tilt = fValue;
		}
		else if (fieldName == "Orbits:") {
			ss >> strValue;
			iValue = celestialIDMap[strValue];
			((Orbital *) celestial)->orbitObject = celestials[iValue];
		}
		else if (fieldName == "Texture:") {
			ss >> strValue;
			celestial->AddTexture(strValue.c_str());
		}
		else if (fieldName == "Eccentricity:") {
			ss >> fValue;
			((Orbital *) celestial)->eccentricity = fValue;
		}
		else if (fieldName == "Period-Offset:") {
			ss >> fValue;
			((Orbital *) celestial)->period_offset = fValue * 60 * 60 * 1000000;
		}
		else if (fieldName == "Rotation-Period:") {
			ss >> fValue;
			celestial->rotation_period = fValue * 60 * 60 * 1000000;
		}
		else if (fieldName == "Shape:") {
			ss >> strValue;
			if (strValue == "sphere")
				celestial->shape = SPHERE;
			else if (strValue == "plane")
				celestial->shape = PLANE;
		}
		else if (fieldName == "Light:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			celestial->light = glm::vec4(floats, 1.0f);
		}
	}
	
	file.close();
	Texture::resetBaseDir();
}

Camera * getCameraFromFile(const char * filename)
{
	std::string line, fieldName;
	std::stringstream ss;
	Camera * camera = NULL;

	float fValue;
	glm::vec3 floats;

	// Open file
	std::ifstream file(filename);
	if (!file.is_open()) {
		printf("File %s failed to open. Exiting parse function.\n", filename);
		return NULL;
	}

	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;

		if (line == "[Camera]") {
			camera = new Camera();
			continue;
		}
		
		if (!camera)
			continue;
		else if (line[0] == '[')
			break;
		
		ss.clear();
		ss.str(line);
		ss >> fieldName;
		if (fieldName == "Position:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			camera->pos = floats;
		}
		else if (fieldName == "Direction:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			camera->dir = floats;
		}
		else if (fieldName == "Up:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			camera->up = floats;
		}
		else if (fieldName == "FOV:") {
			ss >> fValue;
			camera->fov = glm::radians(fValue);
		}
		else if (fieldName == "Near:") {
			ss >> fValue;
			camera->near = fValue;
		}
		else if (fieldName == "Far:") {
			ss >> fValue;
			camera->far = fValue;
		}
	}

	file.close();
	
	return camera;
}

SkyArray * getSkyArrayFromFile(const char * filename)
{
	std::string line, fieldName;
	std::stringstream ss;

	SkyArray * skyArray = new SkyArray();
	Sky * sky = NULL;
	
	bool hasColor = false;
	bool hasTexture = false;
	bool hasAngles = false;
	std::string textureFilename;
	glm::vec3 color;
	std::vector<float> angles;

	float fValue;
	glm::vec3 floats;
	std::string strValue;

	// Open file
	std::ifstream file(filename);
	if (!file.is_open()) {
		printf("File %s failed to open. Exiting parse function.\n", filename);
		return NULL;
	}

	Texture::assignBaseDir(filename);
	
	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;

		if (line == "[BackgroundTexture]") {
			sky = new SkyPanorama();
			continue;
		}
		
		else if (line == "[BackgroundColor]") {
			sky = new SkyColor();
			continue;
		}

		// New object that is not recognized by this function
		else if (line[0] == '[') {
			sky = NULL;
			hasColor = hasTexture = hasAngles = false;
		}

		if (!sky)
			continue;
		
		// Get next word from line, which represents name of field. If field is not reconized continue to next line
		// Get next word from line, which represents value of field.
		ss.clear();
		ss.str(line);
		ss >> fieldName;
		if (fieldName == "Texture:") {
			ss >> strValue;
			sky->Init(strValue.c_str());
			hasTexture = true;
			
			if (hasAngles)
				for (unsigned int i = 0; i < angles.size(); i++)
					skyArray->AddSky(sky, angles[i]);
		}
		if (fieldName == "Color:") {
			ss >> floats[0] >> floats[1] >> floats[2];
			sky->Init(floats);
			hasColor = true;
			
			if (hasAngles)
				for (unsigned int i = 0; i < angles.size(); i++)
					skyArray->AddSky(sky, angles[i]);
		}
		else if (fieldName == "Angles:") {
			while (!ss.eof()) {
				ss >> fValue;
				if (ss.fail())
					break;
				angles.push_back(fValue);
				hasAngles = true;
			}
			
			if (hasTexture || hasColor) {
				for (unsigned int i = 0; i < angles.size(); i++)
					skyArray->AddSky(sky, angles[i]);
				angles.clear();
				hasAngles = false;
			}
		}
		
	}
	file.close();
	Texture::resetBaseDir();
	return skyArray;
}

std::string getStarboxFromFile(const char * filename)
{
	std::string line, fieldName;
	std::stringstream ss;
	bool foundStarbox = false;
	std::string textureFilename;
	
	// Open file
	std::ifstream file(filename);
	if (!file.is_open()) {
		printf("File %s failed to open. Exiting parse function.\n", filename);
		return "";
	}

	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;

		if (line == "[BackgroundTexture]") {
			foundStarbox = true;
			continue;
		}
		
		if (!foundStarbox)
			continue;
		else if (line[0] == '[')
			break;
		
		ss.clear();
		ss.str(line);
		ss >> fieldName;
		if (fieldName == "Texture:") {
			ss >> textureFilename;
		}
	}

	file.close();
	
	return textureFilename;
}

#include <fstream>
#include <sstream>

#include "parser.h"
#include "celestial.h"

void parseSystemFile(std::vector<Celestial *> & celestials, const char * filename)
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
		printf("File %s failed to open. Exiting parse function.\n", filename);
		return;
	}

	while (!file.eof()) {
		// Get line
		getline(file, line);
	
		// If line is comment or empty then continue to next line
		if (line.size() < 1 || line[0] == '#')
			continue;
	
		// If line is recognized celestial type, then create new celestial. If not recognized then continue to next line
		if (line == "[Celestial]") {
			if (celestial)
				celestial->Init();
			celestial = new Celestial();
			celestials.push_back(celestial);
			continue;
		}
		else if (line == "[Orbital]") {
			if (celestial)
				celestial->Init();
			celestial = new Orbital();
			celestials.push_back(celestial);
			continue;
		}
		
		if (!celestial)
			continue;

		// Get next word from line, which represents name of field. If field is not reconized continue to next line
		// Get next word from line, which represents value of field.
		ss.clear();
		ss.str(line);
		ss >> fieldName;
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
			celestial->program = new GLProgram("shaders/celestial.vert", NULL, "shaders/celestial.tes", strValue.c_str());
		}
		else if (fieldName == "Distance:") {
			ss >> fValue;
			((Orbital *) celestial)->distance = fValue;
		}
		else if (fieldName == "Period:") {
			ss >> fValue;
			((Orbital *) celestial)->period = fValue;
		}
		else if (fieldName == "Tilt:") {
			ss >> fValue;
			celestial->tilt = fValue;
		}
		else if (fieldName == "Orbits:") {
			// TODO: This is a temporary fix so that orbits can be implemented quickly.
			// Should use names instead of object indexes.
			ss >> iValue;
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
			((Orbital *) celestial)->period_offset = fValue;
		}
		else if (fieldName == "Rotation-Period:") {
			ss >> fValue;
			celestial->rotation_period = fValue;
		}
	}
	
	if (celestial)
		celestial->Init();

	file.close();
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

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>
#include <string>
#include "celestial.h"
#include "camera.h"
#include "skypanorama.h"

extern std::map<std::string, int> celestialIDMap;

void parseSystemFile(std::vector<Celestial *> & celestials, const char * filename, const char * vertexshaderfile = "shaders/celestialV.vert");
celestial::Camera * getCameraFromFile(const char * filename);
std::string getStarboxFromFile(const char * filename);
SkyArray * getSkyArrayFromFile(const char * filename);

#endif // PARSER_H

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "celestial.h"
#include "camera.h"

void parseSystemFile(std::vector<Celestial *> & celestials, const char * filename, const char * vertexshaderfile = "shaders/celestialV.vert", const char * tessEshaderfile = NULL);
Camera * getCameraFromFile(const char * filename);

#endif // PARSER_H
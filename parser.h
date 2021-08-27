#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "celestial.h"
#include "camera.h"

void parseSystemFile(std::vector<Celestial *> & celestials, const char * filename);
Camera * getCameraFromFile(const char * filename);

#endif // PARSER_H

#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <string>

#include "glprogram.h"
#include "texture.h"

Texture getTexture(const std::string & filename);

GLShader * getShader(const std::string filename, GLenum shaderType);

GLProgram * getProgram(const char * vertexshader, const char * fragmentshader);
GLProgram * getProgram(const char * vertexshader, const char * geometryshader, const char * fragmentshader);
GLProgram * getProgram(const char * vertexshader, const char * tessCShader,
	const char * tessEShader, const char * fragmentshader);

#endif // RESOURCE_HANDLER_H

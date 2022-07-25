#include <map>
#include <sstream>

#include "resource-handler.h"

#ifndef RESOURCE_BASE_DIR
	#define RESOURCE_BASE_DIR ""
#endif

static std::map<std::string, Texture> textures;
static std::map<std::string, GLShader *> shaders;
static std::map<std::string, GLProgram *> programs;

Texture getTexture(const std::string & filename)
{
	// Search map for filename
	auto textureIter = textures.find(filename);

	if (textureIter != textures.end()) {
		// If found, return Texture
		return textureIter->second;
	}
	else {
		// else create and initialize and return new texture
		Texture texture;
		texture.Init(filename);
		textures[filename] = texture;
		return texture;
	}
}

GLShader * getShader(const std::string filename, GLenum shaderType)
{
	auto shaderIter = shaders.find(filename);
	
	if (shaderIter != shaders.end())
		// If found, return Shader
		return shaderIter->second;
	else {
		// else return Shader
		GLShader * shader = new GLShader(filename.c_str(), shaderType);
		shaders[filename] = shader;
		return shader;
	}
}

GLProgram * getProgramInternal(const std::string key)
{
	// Search map for filename
	auto programIter = programs.find(key);

	if (programIter != programs.end())
		// If found, return Program
		return programIter->second;
	else
		// Program not found. Creation below.
		return NULL;
}

GLProgram * getProgram(const char * vs, const char * fs, bool useBaseDir)
{
	std::string vertexshader = RESOURCE_BASE_DIR;
	vertexshader += vs;
	std::string fragmentshader = RESOURCE_BASE_DIR;
	if (useBaseDir)
		fragmentshader += fs;
	else
		fragmentshader = fs;

	std::stringstream key; key << vertexshader << "/" << fragmentshader;

	GLProgram * program = getProgramInternal(key.str());
	if (!program) {
		program = new GLProgram(vertexshader.c_str(), fragmentshader.c_str());
		programs[key.str()] = program;
	}
	return program;
}

GLProgram * getProgram(const char * vs, const char * gs, const char * fs, bool useBaseDir)
{
	std::string vertexshader = RESOURCE_BASE_DIR;
	vertexshader += vs;
	std::string geometryshader = RESOURCE_BASE_DIR;
	geometryshader += gs;
	std::string fragmentshader = RESOURCE_BASE_DIR;
	if (useBaseDir)
		fragmentshader += fs;
	else
		fragmentshader = fs;

	std::stringstream key; key << vertexshader << "/" << geometryshader << "/" << fragmentshader;

	GLProgram * program = getProgramInternal(key.str());
	if (!program) {
		program = new GLProgram(vertexshader.c_str(), geometryshader.c_str(), fragmentshader.c_str());
		programs[key.str()] = program;
	}
	return program;
}

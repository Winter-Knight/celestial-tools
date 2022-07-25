#include "glprogram.h"

const unsigned int BUFFER_SIZE = 32768;

int GLShader::CheckGLShader(const char * str)
{
	int status, len;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	printf("%s - OpenGL Shader Compile Status: %d\n", str, status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 1) {
		GLchar buffer[BUFFER_SIZE];
		glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, buffer);
		printf("%s - OpenGL Shader Info Log:\n%s\n", str, buffer);
	}
	return !status;
}

int GLProgram::CheckGLProgram(const char * str)
{
	int validateStatus, linkStatus, len;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &validateStatus);
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	int error = (!validateStatus || !linkStatus || len > 1);
	printf("%s - OpenGL Program Status: %d %d %d\n", str, validateStatus, linkStatus, len);
	if (error) {
		GLchar buffer[BUFFER_SIZE];
		glGetProgramInfoLog(program, BUFFER_SIZE, NULL, buffer);
		printf("%s - OpenGL Program Info Log:\n%s\n", str, buffer);
	}
	return error;
}

int GLShader::SourceFile(std::string & shaderbuffer, const std::string & shaderfile)
{
	std::string line;
	size_t begin, end;
	int error;

	std::ifstream infile(shaderfile);
	
	if (infile.fail()) {
		printf("Error: Could not open %s!\n", shaderfile.c_str());
		return 1;
	}

	while (std::getline(infile, line)) {
		if (line.find("#include") == 0) {
			begin = line.find('<') + 1;
			end = line.find('>', begin) - begin;

			std::string relativePath = shaderfile;
			unsigned int lastSlash = relativePath.find_last_of("/\\");
			if (lastSlash != std::string::npos)
				relativePath = relativePath.substr(0, lastSlash + 1);
			else
				relativePath = "";

			if (SourceFile(shaderbuffer, relativePath + line.substr(begin, end))) {
				infile.close();
				return 1;
			}
		}
		else
			shaderbuffer += line += "\n";
	}

	infile.close();
	return 0;
}

GLShader::GLShader(const char * shaderfile, GLenum shadertype)
{
	std::string shaderbuffer;
	const GLchar * pbuffer;

	if (SourceFile(shaderbuffer, shaderfile))
		return;
	pbuffer = shaderbuffer.c_str();

	shader = glCreateShader(shadertype);
	glShaderSource(shader, 1, &pbuffer, NULL);
	glCompileShader(shader);
	CheckGLShader(shaderfile);
}

void GLProgram::AddShader(const char * shaderfile, GLenum shadertype)
{
	if (shaderfile) {
		GLShader * shader = getShader(shaderfile, shadertype);
		glAttachShader(program, shader->getShaderID());
	}
}

GLProgram::GLProgram(const char * vertexshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, fragmentshaderfile);
}

GLProgram::GLProgram(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, geometryshaderfile, fragmentshaderfile);
}

GLProgram::GLProgram(const char * vertexshaderfile, const char * tessCshaderfile,
		const char * tessEshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, tessCshaderfile, tessEshaderfile, fragmentshaderfile);
}


void GLProgram::Init(const char * vertexshaderfile, const char * fragmentshaderfile)
{
	program = glCreateProgram();

	AddShader(vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(fragmentshaderfile, GL_FRAGMENT_SHADER);

	glLinkProgram(program);
	CheckGLProgram("OpenGL Program");
}

void GLProgram::Init(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile)
{
	program = glCreateProgram();

	AddShader(vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(geometryshaderfile, GL_GEOMETRY_SHADER);
	AddShader(fragmentshaderfile, GL_FRAGMENT_SHADER);

	glLinkProgram(program);
	CheckGLProgram("OpenGL Program");
}

void GLProgram::Init(const char * vertexshaderfile, const char * tessCshaderfile,
	const char * tessEshaderfile, const char * fragmentshaderfile)
{
	program = glCreateProgram();

	AddShader(vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(tessCshaderfile, GL_TESS_CONTROL_SHADER);
	AddShader(tessEshaderfile, GL_TESS_EVALUATION_SHADER);
	AddShader(fragmentshaderfile, GL_FRAGMENT_SHADER);

	glLinkProgram(program);
	CheckGLProgram("OpenGL Program");
}

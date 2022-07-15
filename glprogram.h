#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H

#include <string>
#include <fstream>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

const unsigned int BUFFER_SIZE = 32768;

class GLShader {
public:
	GLShader(const char * shaderfile, GLenum shaderType);

	~GLShader() {
		glDeleteShader(shader);
		printf("Shader %d Deleted\n", shader);
	}

	GLuint getShaderID() { return shader; }

private:
	int SourceFile(std::string & shaderbuffer, const char * shaderfile);
	int CheckGLShader(const char * str = "");

	GLuint shader;
};

// Prevent circular dependencies
extern GLShader * getShader(const std::string filename, GLenum shaderType);

class GLProgram {
public:

	GLProgram(const char * vertexshaderfile, const char * fragmentshaderfile);
	GLProgram(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile);
	GLProgram(const char * vertexshaderfile, const char * tessCshaderfile,
		const char * tessEshaderfile, const char * fragmentshaderfile);

	void Init(const char * vertexshaderfile, const char * fragmentshaderfile);
	void Init(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile);
	void Init(const char * vertexshaderfile, const char * tessCshaderfile,
		const char * tessEshaderfile, const char * fragmentshaderfile);

	~GLProgram() {
		glDeleteProgram(program);
		printf("Program %d Deleted\n", program);
	}

	void Use() { glUseProgram(program); }

	GLuint getProgramID() { return program; }

	void SetUniformi(const char * name, GLint value)
	{
		glUniform1i(glGetUniformLocation(program, name), value);
	}

	void SetUniformf(const char * name, GLfloat value)
	{
		glUniform1f(glGetUniformLocation(program, name), value);
	}

	void SetUniform2f(const char * name, GLfloat * values)
	{
		glUniform2fv(glGetUniformLocation(program, name), 1, values);
	}

	void SetUniform3f(const char * name, GLfloat * values)
	{
		glUniform3fv(glGetUniformLocation(program, name), 1, values);
	}

	void SetUniform4f(const char * name, GLfloat * values)
	{
		glUniform4fv(glGetUniformLocation(program, name), 1, values);
	}

	void SetUniformMatrix4f(const char * name, GLfloat * values)
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, values);
	}

	int CheckGLProgram(const char * str = "");

private:

	void AddShader(const char * shaderfile, GLenum shadertype);

	GLuint program;
};

inline int GLShader::CheckGLShader(const char * str)
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

inline int GLProgram::CheckGLProgram(const char * str)
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

inline int GLShader::SourceFile(std::string & shaderbuffer, const char * shaderfile)
{
	std::string line;
	size_t begin, end;
	int error;

	std::ifstream infile(shaderfile);
	
	if (infile.fail()) {
		printf("Error: Could not open %s!\n", shaderfile);
		return 1;
	}

	while (std::getline(infile, line)) {
		if (line.find("#include") == 0) {
			begin = line.find('<') + 1;
			end = line.find('>', begin) - begin;
			if (SourceFile(shaderbuffer, line.substr(begin, end).c_str())) {
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

inline GLShader::GLShader(const char * shaderfile, GLenum shadertype)
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

inline void GLProgram::AddShader(const char * shaderfile, GLenum shadertype)
{
	if (shaderfile) {
		GLShader * shader = getShader(shaderfile, shadertype);
		glAttachShader(program, shader->getShaderID());
	}
}

inline GLProgram::GLProgram(const char * vertexshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, fragmentshaderfile);
}

inline GLProgram::GLProgram(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, geometryshaderfile, fragmentshaderfile);
}

inline GLProgram::GLProgram(const char * vertexshaderfile, const char * tessCshaderfile,
		const char * tessEshaderfile, const char * fragmentshaderfile)
{
	Init(vertexshaderfile, tessCshaderfile, tessEshaderfile, fragmentshaderfile);
}


inline void GLProgram::Init(const char * vertexshaderfile, const char * fragmentshaderfile)
{
	program = glCreateProgram();

	AddShader(vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(fragmentshaderfile, GL_FRAGMENT_SHADER);

	glLinkProgram(program);
	CheckGLProgram("OpenGL Program");
}

inline void GLProgram::Init(const char * vertexshaderfile, const char * geometryshaderfile, const char * fragmentshaderfile)
{
	program = glCreateProgram();

	AddShader(vertexshaderfile, GL_VERTEX_SHADER);
	AddShader(geometryshaderfile, GL_GEOMETRY_SHADER);
	AddShader(fragmentshaderfile, GL_FRAGMENT_SHADER);

	glLinkProgram(program);
	CheckGLProgram("OpenGL Program");
}

inline void GLProgram::Init(const char * vertexshaderfile, const char * tessCshaderfile,
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

#endif // GL_PROGRAM_H

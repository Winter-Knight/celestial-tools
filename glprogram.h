#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H

#include <string>
#include <fstream>
#include <vector>

#ifndef _WIN32
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
#endif // _WIN32

class GLShader {
public:
	GLShader(const char * shaderfile, GLenum shaderType);

	~GLShader() {
		glDeleteShader(shader);
		printf("Shader %d Deleted\n", shader);
	}

	GLuint getShaderID() { return shader; }

private:
	int SourceFile(std::string & shaderbuffer, const std::string & shaderfile);
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

#endif // GL_PROGRAM_H

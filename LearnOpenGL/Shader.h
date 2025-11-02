#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();
	void use() const;
	GLuint getProgram() const;
	GLint getAndCheckUniformLocation(const char* name) const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

private:
	GLuint programID = 0;
	void checkShaderCompileErrors(unsigned int shader, const std::string& type);
	void checkProgramLinkErrors(unsigned int program);
	GLuint compileShader(GLenum type, const std::string& source);
	GLuint createProgramFromSources(const std::string& vertexSrc, const std::string& fragmentSrc);
	std::string loadFile(const std::string& path);
};

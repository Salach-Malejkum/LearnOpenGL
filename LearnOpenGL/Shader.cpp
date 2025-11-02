#include "Shader.h"

void Shader::checkShaderCompileErrors(unsigned int shader, const std::string& type)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::checkProgramLinkErrors(unsigned int program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

GLuint Shader::compileShader(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	checkShaderCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
	return shader;
}

GLuint Shader::createProgramFromSources(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkProgramLinkErrors(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint Shader::getProgram() const
{
	return programID;
}

GLint Shader::getAndCheckUniformLocation(const char* name) const
{
	GLint location = glGetUniformLocation(this->programID, name);
	if (location == -1) {
		std::cerr << "Warning: uniform '" << name << "' not found (will default to location -1)\n";
	}

	return location;
}

void Shader::use() const
{
	if (programID != 0) glUseProgram(this->programID);
	else std::cerr << "Warning: Shader::use called with invalid programID\n";;
}

std::string Shader::loadFile(const std::string& path)
{
	std::ifstream inputFile(path);
	if (!inputFile) return {};
	std::ostringstream ss;
	ss << inputFile.rdbuf();
	return ss.str();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSrc = loadFile(vertexPath);
	std::string fragmentSrc = loadFile(fragmentPath);

	if (vertexSrc.empty() || fragmentSrc.empty())
	{
		std::cerr << "Failed to load shader files." << std::endl;
		this->programID = 0;
		return;
	}

	this->programID = createProgramFromSources(vertexSrc, fragmentSrc);
}

Shader::~Shader()
{
	glDeleteProgram(this->programID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(getAndCheckUniformLocation(name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(getAndCheckUniformLocation(name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getAndCheckUniformLocation(name.c_str()), value);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(getAndCheckUniformLocation(name.c_str()), x, y, z, w);
}
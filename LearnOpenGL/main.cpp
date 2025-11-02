#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int WIDTH{ 800 };
const int HEIGHT{ 600 };


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


static std::string loadFile(const std::string& path)
{
	std::ifstream inputFile(path);
	if (!inputFile) return {};
	std::ostringstream ss;
	ss << inputFile.rdbuf();
	return ss.str();
}


static void checkShaderCompileErrors(unsigned int shader, const std::string& type)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}


static void checkProgramLinkErrors(unsigned int program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}


static GLuint compileShader(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	checkShaderCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
	return shader;
}


static GLuint createProgramFromSources(const std::string& vertexSrc, const std::string& fragmentSrc)
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


int main()
{
	// GLFW window initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// GLAD load OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	// Prepare shaders
	std::string vertexSrc = loadFile("vertex.glsl");
	std::string fragmentSrc = loadFile("fragment.glsl");

	if (vertexSrc.empty() || fragmentSrc.empty())
	{
		std::cout << "Failed to load shader files." << std::endl;
		return -1;
	}

	// Shader program
	unsigned int shaderProgram = createProgramFromSources(vertexSrc, fragmentSrc);
	GLint colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	if (colorLocation == -1) {
		std::cerr << "Warning: uniform 'uColor' not found (will default to location -1)\n";
	}

	float vertices[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	float vertices1[] = {
		// second triangle
		-0.5f, -0.5f, 0.0f,
		 0.0f, -0.1f, 0.0f,
		-0.3f,  0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		3, 4, 5  // second triangle
	};

	unsigned int EBO;
	unsigned int VBO[2], VAO[2];
	
	// Initialize VAO, VBO
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	// VAO
	glBindVertexArray(VAO[0]);

	// VBO
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// VAO1
	glBindVertexArray(VAO[1]);

	// VBO1;
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// orange triangle
		if (colorLocation != -1) glUniform4f(colorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// yellow triangle
		if (colorLocation != -1) glUniform4f(colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
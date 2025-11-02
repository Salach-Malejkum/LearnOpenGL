#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"

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

	// Shader program
	Shader shaderProgram("vertex.glsl", "fragment.glsl");

	float vertices[] = {
		// first triangle  // colors
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

		shaderProgram.use();

		// orange triangle
		//shaderProgram.setVec4("uColor", 1.0f, 1.0f, 0.2f, 1.0f);
		shaderProgram.setBool("uUseColor", false);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// yellow triangle
		shaderProgram.setVec4("uColor", 1.0f, 1.0f, 0.0f, 1.0f);
		shaderProgram.setBool("uUseColor", true);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();
	return 0;
}
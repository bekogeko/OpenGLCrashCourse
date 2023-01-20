#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include "VAO.h"
#include "EBO.h"


int main() {

	// Init GLFW
	glfwInit();

	// specify Opengl Versions and Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f,     -0.5f * float(sqrt(3)) / 3,      0.0f,	 0.8f,  0.3f,   0.02f,		// Lower left corner
		0.5f,      -0.5f * float(sqrt(3)) / 3,      0.0f,    0.8f,  0.3f,   0.02f, 	// Lower right corner
		0.0f,       0.5f * float(sqrt(3)) * 2 / 3,  0.0f,    1.0f,  0.6f,   0.32f,		// Upper corner
		-0.5f / 2,  0.5f * float(sqrt(3)) / 6,      0.0f,	 0.9f,  0.45f,  0.17f,		// Inner left
		0.5f / 2,   0.5f * float(sqrt(3)) / 6,      0.0f,	 0.9f,  0.45f,  0.17f,		// Inner right
		0.0f,      -0.5f * float(sqrt(3)) / 3,      0.0f,	 0.8f,  0.3f,   0.02f		// Inner down
	};
	GLuint indices[] =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1 // Upper triangle
	};



	// Create a glfw window
	GLFWwindow* window = glfwCreateWindow(600, 600, "Opengl Crash Course", NULL, NULL);

	// if there is no window then report error
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Load Glad so it loads Opengl for us
	gladLoadGL();

	// set viewport for the render in opengl
	glViewport(0, 0, 600, 600);

	Shader shaderProgram("default.vert","default.frag");

	VAO vao1;
	vao1.Bind();
	
	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));

	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();


	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	while (!glfwWindowShouldClose(window))
	{
		// clear bg with color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear color
		glClear(GL_COLOR_BUFFER_BIT);
		
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5);

		vao1.Bind();

		glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT,0 );

		// swap new frame buffer
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	shaderProgram.Delete(); 

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
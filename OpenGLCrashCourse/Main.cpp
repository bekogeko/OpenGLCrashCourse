#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

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

	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	 0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	 1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	 1.0f, 0.0f  // Lower right corner
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
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

	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();


	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	// Texture
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load("texture.png", &widthImg, &heightImg, &numColCh,0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D,0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	while (!glfwWindowShouldClose(window))
	{
		// clear bg with color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear color
		glClear(GL_COLOR_BUFFER_BIT);
		
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5);
		glBindTexture(GL_TEXTURE_2D, texture);

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

	glDeleteTextures(1, &texture);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
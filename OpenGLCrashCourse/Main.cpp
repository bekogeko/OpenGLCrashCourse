#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shaderClass.h"
#include "Texture.h"
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
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
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
	Texture one_eye("texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

	one_eye.texUnit(shaderProgram, "text0", 0);

	float rotation=0;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// clear bg with color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// bind shader for all later extras
		shaderProgram.Activate();

		double currentTime = glfwGetTime();

		if (currentTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = currentTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj= glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f,0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID,"model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		int viewLoc = glGetUniformLocation(shaderProgram.ID,"view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		

		int projLoc = glGetUniformLocation(shaderProgram.ID,"proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


		//assigns a value to the uniform; NOTE: must always be done after activating the shader code
		glUniform1f(uniID, 0.5);
		//binds the texture so that is appears in rendering
		one_eye.Bind();
		
		vao1.Bind();

		glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		// swap new frame buffer
		glfwSwapBuffers(window);

		glfwPollEvents();

		// sets max fps to screen hertz
		glfwSwapInterval(1);
	}

	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	one_eye.Delete();
	shaderProgram.Delete(); 


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
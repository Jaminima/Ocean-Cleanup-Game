#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

void drawWindow() {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//modify position using mv & p
	glBindVertexArray(VAOs[0]);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	//modify position using mv & p
	glBindVertexArray(VAOs[1]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
}

void initWindow() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	InitRenderer();

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		drawWindow();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
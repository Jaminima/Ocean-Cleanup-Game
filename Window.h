#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case 'W':
		sceneObjs.cam.updatePosition(vec3(0, 0, 1));
		break;
	case 'S':
		sceneObjs.cam.updatePosition(vec3(0, 0, -1));
		break;
	case 'A':
		sceneObjs.cam.updatePosition(vec3(1, 0, 0));
		break;
	case 'D':
		sceneObjs.cam.updatePosition(vec3(-1, 0, 0));
		break;
	case 'Z':
		sceneObjs.cam.updatePosition(vec3(0, -1, 0));
		break;
	case 'X':
		sceneObjs.cam.updatePosition(vec3(0, 1, 0));
		break;

	}

	sceneObjs.cam.updateMatrix();
}

void initWindow() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	InitRenderer();

	glfwSetKeyCallback(window, KeyCallback);

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
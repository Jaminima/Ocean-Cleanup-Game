#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float moveStep = 0.1f;

	switch (key) {
	case 'W':
		sceneObjs.cam.updatePosition(vec3(0, 0, moveStep));
		break;
	case 'S':
		sceneObjs.cam.updatePosition(vec3(0, 0, -moveStep));
		break;
	case 'A':
		sceneObjs.cam.updatePosition(vec3(moveStep, 0, 0));
		break;
	case 'D':
		sceneObjs.cam.updatePosition(vec3(-moveStep, 0, 0));
		break;
	case 'Z':
		sceneObjs.cam.updatePosition(vec3(0, -moveStep, 0));
		break;
	case 'X':
		sceneObjs.cam.updatePosition(vec3(0, moveStep, 0));
		break;
	case ']':
		exit(0);
		break;
	}

	sceneObjs.cam.updateMatrix();
}

double mouseX, mouseY;

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	float step = 0.1f;

	double dx = mouseX - xpos;
	double dy = mouseY - ypos;

	sceneObjs.cam.rotation += vec3(dy * step, dx * step, 0);

	sceneObjs.cam.updateMatrix();

	mouseX = xpos;
	mouseY = ypos;
}

void initWindow() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	InitRenderer();

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
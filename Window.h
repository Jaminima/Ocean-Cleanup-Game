#pragma once
#include "Game.h"

void initWindow() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Ocean Cleanup", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	glfwHideWindow(window);

	InitRenderer();

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	system("cls");

	printf("Controls:\nWASD - Move\nX/Z - Up/Down\nMouse - Look\nLeft Click - Pickup Item\n\nR - Restart\n] - Quit\n\n");
	
	printf("The Aim Of The Game Is To Collect Rubbish & Fish From The Ocean.\nHigher The Score The Better!\nMake Sure You Keep An Eye On Your Breath!\n\n");

	printf("Press Any Key To Continue...\n\n");

	cin.ignore();

	glfwShowWindow(window);

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Tick();

		Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
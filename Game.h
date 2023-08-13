#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

struct gameState {
public:
	int score = 0;
	int maxBreath = 20;
	int curBreath = 20;
};

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

	float dist = 0;

	for (auto a : assets) {
		if (a->state.canBeHovered && a->BeamCollides(sceneObjs.cam.position, sceneObjs.cam.lookingAt, &dist)) {
			a->state.hovered = dist < 5;
		}
		else {
			a->state.hovered = false;
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	float dist;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (auto a : assets) {
			if (a->BeamCollides(sceneObjs.cam.position, sceneObjs.cam.lookingAt, &dist)) {
				if (dist < 5) {
					printf("Hit %s\n", a->fileName.c_str());
				}
			}
		}
	}
}
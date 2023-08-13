#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

struct gameState {
public:
	int score = 0;
	int maxBreath = 20;
	int curBreath = 20;
};

vec3 rndDir() {
	return vec3(
		(rand() % 1000 - 499) / 1000000.0f,
		(rand() % 1000 - 499) / 1000000.0f,
		0);
}

void Tick() {
	for (auto f : fish) {
		int stage = f->state.stage;

		int s = ((stage % 1000) - 499) / 100.0f;
		f->rotation += vec3(f->state.turn[0], f->state.turn[1], s * 0.0001f);

		mat4 model(1);

		model = glm::rotate(model, -f->rotation[0], vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -f->rotation[1], vec3(0.0f, 1.0f, 1.0f));
		model = glm::rotate(model, f->rotation[2], vec3(0.0f, 0.0f, 1.0f));

		f->position -= vec3(vec4(vec3(s * 0.0001f, 0, 0.001f),1) * model);

		if (f->position[1] > 0) {
			f->rotation[stage % 3] *= -1;
			f->position[1] -= 1.1f;
		}

		if (f->position[1] < -5) {
			f->rotation[stage % 3] *= -1;
			f->position[1] += 1.1f;
		}

		f->UpdateBounds();

		f->state.stage++;

		if (f->state.stage > 1000) {
			f->state.stage = 0;
			f->state.turn = rndDir();
		}
	}
}

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
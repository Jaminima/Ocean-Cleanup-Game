#pragma once
#include "Render.h"
#include "GLFW/glfw3.h"

struct gameState {
public:
	int rubbishTaken = 0;
	int fishTaken = 0;
	float maxBreath = 20;
	float curBreath = 20;
} _gameState;

vec3 rndDir() {
	return vec3(
		(rand() % 1000 - 499) / 1000000.0f,
		(rand() % 1000 - 499) / 1000000.0f,
		0);
}

void PrintState() {
	float score = _gameState.rubbishTaken + (_gameState.fishTaken / 5.0f);
	printf("Breath %.2f/%.0f -- Score %.1f        \r", _gameState.curBreath, _gameState.maxBreath, score);
}

clock_t gameTime = 0;

void Tick() {
	auto newTime = clock();

	PrintState();

	auto tDif = (newTime - gameTime) / 1000.0f;

	if (sceneObjs.cam.position[1] < 0) {
		_gameState.curBreath -= tDif;
	}
	else {
		_gameState.curBreath += _gameState.curBreath < 20 ? tDif : 0;
	}

	gameTime = newTime;

	for (auto f : assets) {
		if (f->state.assetType != fish || f->state.hidden)
			continue;

		int stage = f->state.stage;

		int s = ((stage % 1000) - 499) / 100.0f;
		f->rotation += vec3(f->state.turn[0], f->state.turn[1], s * 0.0001f);

		mat4 model(1);

		model = glm::rotate(model, -f->rotation[0], vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -f->rotation[1], vec3(0.0f, 1.0f, 1.0f));
		model = glm::rotate(model, f->rotation[2], vec3(0.0f, 0.0f, 1.0f));

		f->position -= vec3(vec4(vec3(s * 0.0001f, 0, 0.001f), 1) * model);

		if (f->position[1] > 0) {
			f->rotation = vec3();
			f->position[1] -= 0.1f;
		}

		if (f->position[1] < -5) {
			f->rotation = vec3();
			f->position[1] += 0.1f;
		}

		f->state.stage++;

		if (f->state.stage > 1000) {
			f->state.stage = 0;
			f->state.turn = rndDir();
			f->UpdateBounds();
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

	if (sceneObjs.cam.position[1] > 1) {
		sceneObjs.cam.position[1] -= 0.1f;
	}

	if (sceneObjs.cam.position[1] < -5) {
		sceneObjs.cam.position[1] += 0.1f;
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

	Asset* closest = 0x0;
	float closestDist = 5;
	float dist = 0;

	for (auto a : assets) {
		a->state.hovered = false;
		if (a->state.canBeHovered && a->BeamCollides(sceneObjs.cam.position, sceneObjs.cam.lookingAt, &dist)) {
			if (closestDist > dist) {
				closest = a;
				closestDist = dist;
			}
		}
	}

	if (closest) {
		closest->state.hovered = true;
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Asset* closest = 0x0;
	float closestDist = 5;
	float dist;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (auto a : assets) {
			if (!a->state.hidden && a->BeamCollides(sceneObjs.cam.position, sceneObjs.cam.lookingAt, &dist)) {
				if (closestDist > dist) {
					closest = a;
					closestDist = dist;
				}
			}
		}
	}

	if (closest) {
		switch (closest->state.assetType) {
		case rubbish:
			closest->state.hidden = true;
			_gameState.rubbishTaken++;
			//printf("Removed Rubbish\n");
			break;

		case fish:
			closest->state.hidden = true;
			_gameState.fishTaken++;
			//printf("Harvested A Fish\n");
			break;

		default:
			//printf("Clicked %s\n", a->fileName.c_str());
			break;
		}
	}
}
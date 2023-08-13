#pragma once
#include "GL.h"

class Camera {
public:
	mat4 projectionMatrix;
	mat4 viewMatrix;

	vec3 position = vec3(0,1,0);
	vec3 rotation = vec3(0, 180, 0);
	vec3 lookingAt;

	float cosx;
	float cosy;
	float cosz;

	float sinx;
	float siny;
	float sinz;

	float fov = 90.0f;
	float aspect = 1.0f;

	float clipNear = 0.1f;
	float clipFar = 1000.0f;

	Camera();

	void updateMatrix();

	void updatePosition(vec3 offset);

	vec3 rotateDirection(vec3 dir);
};

class SceneObjects {
public:
	Camera cam;
};
#include "Scene.h"

#define PI 3.14159f
#define halfPI PI/2
#define DegreesToRadians (PI/180)

Camera::Camera()
{
	projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);

	this->updateMatrix();
}

void boundAngles(vec3* angle)
{
	while (angle->x > 360) angle->x -= 360;
	while (angle->x < 0) angle->x += 360;

	while (angle->y > 360) angle->y -= 360;
	while (angle->y < 0) angle->y += 360;

	while (angle->z > 360) angle->z -= 360;
	while (angle->z < 0) angle->z += 360;
}

void Camera::updateMatrix() {
	boundAngles(&rotation);

	vec3 radianRot = rotation * DegreesToRadians;

	cosx = cosf(radianRot.x);
	cosy = cosf(radianRot.y);
	cosz = cosf(radianRot.z);

	sinx = sinf(radianRot.x);
	siny = sinf(radianRot.y);
	sinz = sinf(radianRot.z);

	vec3 dir = vec3(0, 0, PI);

	lookingAt = rotateDirection(dir);

	viewMatrix = glm::lookAt(position, position + lookingAt, vec3(0, 1, 0));
}

vec3 Camera::rotateDirection(vec3 dir)
{
	dir = vec3(
		dir.x,
		(cosx * dir.y) + (sinx * dir.z),
		(cosx * dir.z) - (sinx * dir.y));

	dir = vec3(
		(cosy * dir.x) + (siny * dir.z),
		dir.y,
		(cosy * dir.z) - (siny * dir.x));

	dir = vec3(
		(cosz * dir.x) + (sinz * dir.y),
		(cosz * dir.y) - (sinz * dir.x),
		dir.z);

	return dir;
}

void Camera::updatePosition(vec3 offset)
{
	//vec3 shift = rotateDirection(vec3(offset.x, offset.z, offset.y));
	vec3 shift = rotateDirection(offset);

	position += vec3(shift);
}
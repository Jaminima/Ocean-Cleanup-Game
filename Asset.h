#pragma once
#ifndef __Asset
#define __Asset
#include "GL.h"
#include "Scene.h"

class AssetData {
public:
	vector<vec3> vertexSet;

	vector<ivec3> indexSet;
};

class Mesh {
public:
	GLuint buffers[bufferCount];
	GLuint vaoBuffer;

	string name;

	vector<vec2> texCooData;

	vector<vec3> vertexData;
	vector<vec3> normalData;

	vector<vec4> colourData;

	Mesh(string name);

	void Build();

	void Render();
};

class Asset {
public:
	string fileName;

	vector<Mesh*>* meshes;

	vec3 rotation = vec3();
	vec3 position = vec3(0,-1,-5);
	vec3 scale = vec3(1);

	Asset(string fileName);

	char* ReadBinaryFile(string filePath, int size);

	void LoadAsset();

	void Build();

	void Render(GLuint programHandle, SceneObjects* sceneObjects);
};

#endif
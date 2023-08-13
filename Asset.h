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

	void GenerateRearFace();
};

enum GameAssetType {
	fish,
	rubbish,
	none
};

class Asset {
public:
	string fileName;

	vector<Mesh*>* meshes;

	vector<GLuint>* textures;

	vec3 rotation = vec3();
	vec3 position = vec3(0, 0, -5);
	vec3 scale = vec3(1);

	vec3 boundSize = vec3();

	struct State {
	public:
		bool hidden = false;
		bool hovered = false;
		bool canBeHovered = true;
		GameAssetType assetType = none;
		int stage = 0;
		vec3 turn = vec3();
	}state;

	Asset(string fileName);

	Asset* Clone();

	char* ReadBinaryFile(string filePath, int size);

	void LoadAsset();

	void Build();

	void Render(GLuint programHandle, SceneObjects* sceneObjects);

	void AddTexture(string filePath);

	void UpdateBounds();

	bool BeamCollides(vec3 origin, vec3 dir, float* dist);

	void GenerateRearFace();
};

#endif
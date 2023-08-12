#ifndef __Render
#define __Render
#include "GL.h"
#include "Asset.h"

vector<Asset*> assets;

SceneObjects sceneObjs;

GLuint program = 0;

void InitRenderer() {
	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);

	glClearColor(0, 0, 0, 1);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	Asset* asset = new Asset("Cube");

	asset->LoadAsset();
	asset->position = vec3(1, 0, -5);

	asset->Build();
	assets.push_back(asset);

	//--------------

	asset = new Asset("Duck");

	asset->LoadAsset();
	asset->position = vec3(-1, 0, -7);
	asset->scale = vec3(0.01);
	asset->AddTexture("DuckCM.png");

	asset->Build();
	assets.push_back(asset);

	//--------------

	asset = new Asset("Avocado");

	asset->LoadAsset();
	asset->position = vec3(-1, -1, -4);
	asset->scale = vec3(50);
	asset->AddTexture("Avocado_baseColor.png");

	asset->Build();
	assets.push_back(asset);

	//--------------

	asset = new Asset("Boat");

	asset->LoadAsset();
	asset->position = vec3(0, 2, -3);
	asset->rotation = vec3(-90, 0, 0);
	asset->scale = vec3(0.1);
	asset->AddTexture("Boat.png");

	asset->Build();
	assets.push_back(asset);
}

void Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto a : assets) {
		a->Render(program, &sceneObjs);
	}
}

#endif
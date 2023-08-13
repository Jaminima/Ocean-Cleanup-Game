#ifndef __Render
#define __Render
#include "GL.h"
#include "Asset.h"
#include <cstdlib>
#include <ctime>

vector<Asset*> assets;

SceneObjects sceneObjs;

GLuint program = 0;

Asset* AddAsset(string assetName, vec3 position = vec3(), vec3 rotation = vec3(), vec3 scale = vec3(1), bool generateRear = false) {
	Asset* a = new Asset(assetName);

	a->LoadAsset();

	a->position = position;
	a->rotation = rotation;
	a->scale = scale;

	if (generateRear)
		a->GenerateRearFace();

	a->Build();

	assets.push_back(a);

	return a;
}

vector<Asset*> fish;

void InitRenderer() {
	srand(std::time(nullptr));

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);

	glClearColor(20, 20, 100, 255);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//--------------

	auto a = AddAsset("Terrain", vec3(0, -5, 0), vec3(0, 0, 0), vec3(50, 5, 50));
	a->AddTexture("material_0_baseColor.png");

	//--------------

	AddAsset("Cube", vec3(1, 0, -5));

	//--------------

	AddAsset("Duck", vec3(3, 0, -7), vec3(), vec3(0.01))
		->AddTexture("DuckCM.png");

	//--------------

	AddAsset("Avocado", vec3(-2, -1, -4), vec3(), vec3(50))
		->AddTexture("Avocado_baseColor.png");

	//--------------

	AddAsset("Boat", vec3(0, 5, -3), vec3(-90, 0, 0), vec3(0.1))
		->AddTexture("Boat.png");

	//--------------

	auto f = AddAsset("BarramundiFish", vec3(-20, -2, -20), vec3(0, 0, 0), vec3(2));
	f->AddTexture("BarramundiFish_baseColor.png");
	fish.push_back(f);

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (x == 0 && y == 0)
				continue;

			auto s = f->Clone();
			s->position += vec3(
				(rand() % 500) / 10.0f,
				((rand() % 40) / -10.0f) - 0.5f,
				(rand() % 500) / 10.0f
			);
			s->rotation = vec3(0,0,0);
			assets.push_back(s);
			fish.push_back(s);
		}
	}

	//--------------

	AddAsset("Newspapers", vec3(5, -2, -3), vec3(0, 0, 0), vec3(0.02))
		->AddTexture("Material_Newspaper_Stack_baseColor.jpeg");

	//--------------

	auto srcSea = AddAsset("Sea", vec3(0, 0, 0), vec3(0, 0, 0), vec3(50), true);
	srcSea->AddTexture("Sea.png");
	srcSea->state.canBeHovered = false;
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto a : assets) {
		a->Render(program, &sceneObjs);
	}
}

#endif
#ifndef __Render
#define __Render
#include "GL.h"
#include "Asset.h"

vector<Asset*> assets;

SceneObjects sceneObjs;

GLuint program = 0;

Asset* AddAsset(string assetName, vec3 position = vec3(), vec3 rotation = vec3(), vec3 scale = vec3(1)) {
	Asset* a = new Asset(assetName);

	a->LoadAsset();

	a->position = position;
	a->rotation = rotation;
	a->scale = scale;

	a->Build();

	assets.push_back(a);

	return a;
}

vector<Asset*> sea;

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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//--------------

	auto a = AddAsset("Terrain", vec3(0, -2, 0), vec3(0, 0, 0), vec3(50));
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

	AddAsset("BarramundiFish", vec3(0, -2, -3), vec3(0, 0, 0), vec3(2))
		->AddTexture("BarramundiFish_baseColor.png");

	//--------------

	AddAsset("Newspapers", vec3(5, -2, -3), vec3(0, 0, 0), vec3(0.02))
		->AddTexture("Material_Newspaper_Stack_baseColor.jpeg");

	//--------------

	auto srcSea = AddAsset("Sea", vec3(0, 0, 0), vec3(0, 0, 0), vec3(50));
	srcSea->AddTexture("Sea.png");
	srcSea->state.canBeHovered = false;
	sea.push_back(srcSea);

	/*for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (x == 0 && y == 0)
				continue;

			auto s = srcSea->Clone();
			s->position += vec3(s->boundSize[2] * x * 1.95f, 0, s->boundSize[2] * y * 1.95f);
			assets.push_back(s);
			sea.push_back(s);
		}
	}*/
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto a : assets) {
		a->Render(program, &sceneObjs);
	}
}

#endif
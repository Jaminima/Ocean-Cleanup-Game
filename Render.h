#ifndef __Render
#define __Render
#include "GL.h"
#include "Asset.h"

Asset a("Cube");

Asset b("Duck");

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

	a.LoadAsset();
	a.position = vec3(1, 0, -5);

	b.LoadAsset();
	b.position = vec3(-1, 0, -7);
	b.scale = vec3(0.01);
	b.AddTexture("DuckCM.png");

	a.Build();
	b.Build();
}

void Render() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	/*glDepthFunc(GL_GEQUAL);
	glEnable(GL_DEPTH_TEST);*/

	a.Render(program, &sceneObjs);
	b.Render(program, &sceneObjs);
}

#endif
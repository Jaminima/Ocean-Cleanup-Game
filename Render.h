#ifndef __Render
#define __Render
#include "GL.h"
#include "Asset.h"

Asset a("Cube");

Asset b("Duck");

Asset c("Avocado");

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

	a.LoadAsset();
	a.position = vec3(1, 0, -5);

	b.LoadAsset();
	b.position = vec3(-1, 0, -7);
	b.scale = vec3(0.01);
	b.AddTexture("DuckCM.png");

	c.LoadAsset();
	c.position = vec3(-1, -1, -4);
	c.scale = vec3(50);
	c.AddTexture("Avocado_baseColor.png");

	a.Build();
	b.Build();
	c.Build();
}

void Render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	a.Render(program, &sceneObjs);
	b.Render(program, &sceneObjs);
	c.Render(program, &sceneObjs);
}

#endif
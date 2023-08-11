
#version 400 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTextureCoordinate;

out vec4 Color;
out vec2 vTextureCoordinate;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

void main()
{
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
	Color = vec4(255);
	vTextureCoordinate = VertexTextureCoordinate;
}

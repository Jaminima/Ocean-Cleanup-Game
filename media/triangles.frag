#version 450 core

in vec4 Color;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;
uniform bool hasTexture;

void main()
{
    FragColor = hasTexture ? texture(faceTexture, vTextureCoordinate) : vec4(255);
}

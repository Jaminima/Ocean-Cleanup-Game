#version 450 core

in vec4 Color;
in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;
uniform bool hasTexture;
uniform bool hovered;

void main()
{
    FragColor = hasTexture ? texture(faceTexture, vTextureCoordinate) : vec4(0,10,10,255);
    FragColor = hovered ? FragColor + vec4(100,100,0,255) : FragColor;
}

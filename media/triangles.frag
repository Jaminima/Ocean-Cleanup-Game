#version 450 core

in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;
uniform bool hasTexture;
uniform bool hovered;
uniform vec3 CameraPos;

void main()
{
    FragColor = hasTexture ? texture(faceTexture, vTextureCoordinate) : vec4(0,10,10,255);
    FragColor = hovered ? FragColor + vec4(100,100,0,255) : FragColor;

    float depth = 1 - (-min(CameraPos[1], 0) / 7);

    FragColor = vec4(FragColor[0] * depth, FragColor[1] * depth, FragColor[2],FragColor[3]);
}

#version 450 core

in vec2 vTextureCoordinate;
layout (location = 0) out vec4 FragColor;

uniform sampler2D faceTexture;
uniform bool hasTexture;
uniform bool hovered;
uniform vec3 CameraPos;

void main()
{
    float depth = 1 - (-min(CameraPos[1], 0) / 7);

    float density = 0.01;

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogFact = clamp(exp2 (-density * density * z * z * 1.442695),0,1);

    FragColor = hasTexture ? texture(faceTexture, vTextureCoordinate) : vec4(0,10,10,255);
    FragColor = hovered ? FragColor + vec4(100,100,0,255) : FragColor;

    vec4 fogCol = vec4(100 * (1-depth),100 * (1-depth),255,0);

    FragColor = mix(fogCol, FragColor, fogFact);
    
    float clampDepth = clamp(0.6, 1, depth);

    FragColor = vec4(FragColor[0] * clampDepth, FragColor[1] * clampDepth, FragColor[2],FragColor[3]);
    
}

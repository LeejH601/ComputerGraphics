#version 440 core

in vec3 WorldPos;
in vec3 Normal;
in vec2 Texcoord0;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 gBaseColor;

void main()
{		
    FragColor = vec4(gBaseColor, 1.0);
}
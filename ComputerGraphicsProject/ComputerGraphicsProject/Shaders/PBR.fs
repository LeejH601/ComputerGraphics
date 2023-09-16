#version 330 core

in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main()
{
	FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
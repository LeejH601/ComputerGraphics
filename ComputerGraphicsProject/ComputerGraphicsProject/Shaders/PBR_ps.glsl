#version 330 core

in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

uniform mat4 gmtxWorld
uniform mat4 gmtxVeiw;
uniform mat4 gmtxProj;

void main()
{
	FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
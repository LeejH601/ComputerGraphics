#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 WorldPos;
out vec3 Normal;

uniform mat4 gmtxWorld
uniform mat4 gmtxVeiw;
uniform mat4 gmtxProj;

void main()
{
	gl_Position = projectionTransform * viewTransform * gmtxWorld * vec4(vPosition, 1.0);
	WorldPos = vec3(gmtxWorld * vec4(vPosition, 1.0);)
	Normal = mat3(transpose(inverse(gmtxWorld))) * vNormal;
}
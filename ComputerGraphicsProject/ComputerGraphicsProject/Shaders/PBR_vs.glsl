#version 330 core
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_texcoord0;

out vec3 WorldPos;
out vec3 Normal;
out vec2 Texcoord0;

uniform mat4 projectionTransform;
uniform mat4 worldTransform;
uniform mat4 viewTransform;


void main()
{
	gl_Position = projectionTransform * viewTransform * worldTransform * vec4(v_Position, 1.0);
	WorldPos = vec3(worldTransform * vec4(v_Position, 1.0));
	Normal = mat3(transpose(inverse(worldTransform))) * v_Normal;
	Texcoord0 = v_texcoord0;
}
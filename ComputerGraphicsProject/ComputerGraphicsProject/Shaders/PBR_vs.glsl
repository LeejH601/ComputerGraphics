#version 330 core
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_texcoord0;
in vec3 v_Tangent;
in vec3 v_Bitangent;

out vec3 WorldPos;
out vec3 Normal;
out vec2 Texcoord0;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 projectionTransform;
uniform mat4 worldTransform;
uniform mat4 viewTransform;


void main()
{
	gl_Position = projectionTransform * viewTransform * worldTransform * vec4(v_Position, 1.0);
	WorldPos = vec3(worldTransform * vec4(v_Position, 1.0));
	Normal = mat3(transpose(inverse(worldTransform))) * v_Normal;
	Texcoord0 = v_texcoord0 * 5.0f;

	mat3 mtxTangentToWorld;
	mtxTangentToWorld[0] = normalize(worldTransform * vec4(v_Tangent, 0.0)).xyz;
	mtxTangentToWorld[2] = normalize(worldTransform * vec4(v_Normal, 0.0)).xyz;
	mtxTangentToWorld[1] = normalize(worldTransform * vec4(cross(v_Normal, v_Tangent), 0.0)).xyz;

	Tangent = mat3(transpose(inverse(worldTransform))) * v_Tangent;
	Bitangent = mat3(transpose(inverse(worldTransform))) * v_Bitangent;
}
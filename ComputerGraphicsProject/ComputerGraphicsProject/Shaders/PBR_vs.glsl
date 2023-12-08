#version 440 core
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_texcoord0;
in vec3 v_Tangent;
in vec3 v_Bitangent;
in ivec4 v_BoneIndices;
in vec4 v_BoneWeights;

out vec3 WorldPos;
out vec3 Normal;
out vec2 Texcoord0;
out vec3 Tangent;
out vec3 Bitangent;
out vec4 LightSpacePos;

uniform mat4 projectionTransform;
uniform mat4 worldTransform;
uniform mat4 viewTransform;
uniform mat4 lightSpaceMatrix;

uniform vec4 gUVOffset;

uniform int gMeshType;

#define MAX_VERTEX_INFLUENCES			4
#define SKINNED_ANIMATION_BONES			128


layout(std140, binding = 1) uniform BindBoneOffset
{
	mat4 gmtxBindBoneOffsets[SKINNED_ANIMATION_BONES];
};

layout(std140, binding = 2) uniform BoneTransforms
{
	mat4 gmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

void main()
{
	mat4 matWorld = mat4(0.0, 0.0, 0.0, 0.0,  
                  0.0, 0.0, 0.0, 0.0, 
                  0.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 0.0);
	if(gMeshType == 1){
		for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
			{
				matWorld += v_BoneWeights[i] * gmtxBindBoneOffsets[v_BoneIndices[i]];
			}
			//matWorld =  mat4(0.001, 0.0, 0.0, 0.0,  
            //      0.0, 0.001, 0.0, 0.0, 
            //      0.0, 0.0, 0.001, 0.0,
            //      0.0, 0.0, 0.0, 1) * matWorld;
			matWorld = worldTransform * matWorld;
	}
	else
		matWorld += worldTransform;
	
	gl_Position = projectionTransform * viewTransform * matWorld * vec4(v_Position, 1.0);
	WorldPos = vec3(matWorld * vec4(v_Position, 1.0));
	Normal = mat3(transpose(inverse(matWorld))) * v_Normal;
	Texcoord0 = v_texcoord0 * gUVOffset.zw;
	//Texcoord0 = Texcoord0 + gUVOffset.xy;

	mat3 mtxTangentToWorld;
	mtxTangentToWorld[0] = normalize(matWorld * vec4(v_Tangent, 0.0)).xyz;
	mtxTangentToWorld[2] = normalize(matWorld * vec4(v_Normal, 0.0)).xyz;
	mtxTangentToWorld[1] = normalize(matWorld * vec4(cross(v_Normal, v_Tangent), 0.0)).xyz;

	Tangent = mat3(transpose(inverse(matWorld))) * v_Tangent;
	Bitangent = mat3(transpose(inverse(matWorld))) * v_Bitangent;
	LightSpacePos = lightSpaceMatrix * vec4(WorldPos, 1.0);
}
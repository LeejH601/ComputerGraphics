#version 330 core

in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;


void main()
{
	//FragColor = vec4(WorldPos,1.0f);

	vec3 vToLight = normalize(vec3(1, 1, 0));
	float fDiffuseFactor = dot(vToLight, Normal);

	vec4 cColor = vec4(1,0,0,1.0f);
	//cColor.rgb *= fDiffuseFactor;
	//cColor.rgb *= vec3(1,1,1);
	//cColor.rgb *= WorldPos;
	FragColor = cColor;
}
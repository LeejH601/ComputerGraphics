#version 330 core

#define MATERIAL_BASE_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_ROUGHNESS_MAP		0x20

in vec3 WorldPos;
in vec3 Normal;
in vec2 Texcoord0;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

const float c_PI = 3.141592;

struct Light
{
	vec3 vec3LightColor;
	float fIntensity;
	vec3 vec3Direction;
};

uniform int gTextureMask;

uniform Light gMainLight;	
uniform vec3 CameraPosition;

uniform vec3 gBaseColor;

uniform float gRoughnessColor;
uniform float gFresnel;
uniform float gMetallicColor;
uniform vec3 gSpecularColor;


uniform sampler2D u_BaseTexture;
uniform sampler2D u_NormalTexture;


vec3 aces_approx(vec3 v)
{
    v *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v * (a * v + b)) / (v * (c * v + d) + e), 0.0f, 1.0f);
}

float GGX_Trowbridge_Reitz(vec3 n, vec3 h, float r)
{
	float a = pow(r, 2);
	
	float NdotH = dot(n, h);
	return pow(a , 2) / ( c_PI * pow( pow(NdotH, 2) * (pow(a , 2) - 1) + 1 ,2 ) );

}


float G_Schlick_GGX(float NdotV, float r)
{
	float a = pow(r, 2);

	float k = a / 2;

	return NdotV / ( NdotV * (1 - k) + k );
}

float F_Cook_Torrance(vec3 v, vec3 h, float F0)
{
	float VdotH = dot(v, h);
	return F0 + (1 - F0) * pow( 1 - VdotH , 5);
}

float Vis_SmithJoint(float a, float NdotV, float NdotL) 
{
	float a2 = pow(a , 2);

	float Vis_SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * a2) + a2);
	float Vis_SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * a2) + a2);
	return 0.5 * (1.f / (Vis_SmithV + Vis_SmithL));
}

//#define USE_VIS

vec3 Cook_Torrance_BRDF(vec3 FinalColor, vec3 BaseColor, vec3 sColor, vec3 normal, vec3 ToLight, vec3 LightColor, float Fresnel, float Roughness, float MetallicColor)
{
	vec3 Diffuse;

	vec3 view = normalize(CameraPosition - WorldPos);

	float NdotL = dot(normal, ToLight);
	float NdotV = max(0.00001f, dot(normal, view));

	float Lambert = max(NdotL / c_PI, 0.0f);
	Diffuse = Lambert * BaseColor * LightColor;
	

	vec3 halfv = normalize(ToLight + normal);

	float D = GGX_Trowbridge_Reitz(normal, halfv, Roughness);
	float G = G_Schlick_GGX(NdotV, Roughness);

	float F0 = pow( (Fresnel - 1) / (Fresnel + 1) ,2 );
	float F = F_Cook_Torrance(view, halfv, F0) * NdotL;


	float Specular;

	#ifdef USE_VIS
	Specular = D * Vis_SmithJoint(Roughness, NdotV ,NdotL) * F;
	#else 
	Specular = (D * G * F) / (4 * NdotL * NdotV);
	#endif 
	
	FinalColor += mix(Diffuse, vec3(Specular) * sColor * LightColor, MetallicColor) ;
	FinalColor = max(vec3(0.0f), FinalColor);


	return FinalColor;
};



void main()
{
	vec3 vToLight = -normalize(gMainLight.vec3Direction);

	vec4 cColor = vec4(0,0,0,1.0f);

	vec3 BaseColor = gBaseColor;
	vec3 SpecularColor = gSpecularColor;
	float Fresnel = gFresnel;
	float Roughness = gRoughnessColor;
	float MetallicColor = gMetallicColor;

	MetallicColor = max(0.01f, MetallicColor);
	
	BaseColor = texture(u_BaseTexture, Texcoord0).xyz;
	//BaseColor = vec3(Texcoord0.x,Texcoord0.y,0);
	//BaseColor.xyz *= 2;

	
	vec3 N = normalize(Normal);
	vec3 T = normalize(Tangent);
	vec3 B = normalize(Bitangent);

	mat3 TBN = mat3(T, B, N);

	vec3 normal = texture(u_NormalTexture, Texcoord0).xyz;
	normal = (2.0f * normal) - 1.0f;

	vec3 normalTBN =  TBN * normal;


	float gamma = 2.2;

	BaseColor = pow(BaseColor, vec3(gamma) );

	vec3 LightColor = gMainLight.vec3LightColor * 1.0f;
	cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, SpecularColor, normalize(normalTBN), vToLight, LightColor, Fresnel, Roughness, MetallicColor);

	float S = 1.0;
	cColor.rgb = vec3(S * aces_approx(cColor.xyz * 0.8));
	
	cColor.rgb = pow(cColor.rgb, vec3(1.0 / gamma));
	
	FragColor = cColor;
	//FragColor = vec4(normalTBN, 1.0f);
}
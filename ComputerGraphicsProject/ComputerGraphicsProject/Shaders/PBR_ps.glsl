#version 440 core

#define MATERIAL_BASE_MAP			0x01
#define MATERIAL_ROUGHNESS_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_SPECULAR_MAP		0x20
#define MAX_LIGHTS					8

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
	uint nLightType;
	vec3 vec3Position;
	float fPacking;
};

layout(std140, binding = 0) uniform Lights
{
	Light g_lights[MAX_LIGHTS];
	uint g_nLights;
};

uniform int gTextureMask;

uniform Light gMainLight;	

uniform vec3 CameraPosition;

uniform vec3 gBaseColor;

uniform float gRoughnessColor;
uniform float gFresnel;
uniform float gMetallicColor;
uniform vec3 gSpecularColor;
//uniform vec4 gUVOffset;

uniform sampler2D u_BaseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;
uniform sampler2D u_EmissionTexture;
uniform samplerCube u_IrradianceTexture;
uniform sampler2D u_BrdfLUT;
uniform samplerCube u_PreFilterMap;

#define TYPE_LIGHT_DIRECTION 0
#define TYPE_LIGHT_DIRECTION_BY_POSITION 1

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

	float k = pow(r + 1, 2) / 8;

	return NdotV / ( NdotV * (1 - k) + k );
}

float GeometrySmith(vec3 n, vec3 v, vec3 l, float r)
{
	float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);
    float ggx1 = G_Schlick_GGX(NdotV, r);
    float ggx2 = G_Schlick_GGX(NdotL, r);
	
    return ggx1 * ggx2;
}

vec3 F_Cook_Torrance(vec3 v, vec3 h, vec3 F0)
{
	float VdotH = dot(v, h);
	return F0 + (1 - F0) * pow( 1 - VdotH , 5);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float Roughness)
{
    return F0 + (max(vec3(1.0 - Roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

float Vis_SmithJoint(float a, float NdotV, float NdotL) 
{
	float a2 = pow(a , 2);

	float Vis_SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * a2) + a2);
	float Vis_SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * a2) + a2);
	return 0.5 * (1.f / (Vis_SmithV + Vis_SmithL));
}

//#define USE_VIS
#define USE_PRECOMPUTED_BRDF

vec3 Cook_Torrance_BRDF(vec3 FinalColor, vec3 BaseColor, vec3 sColor, vec3 normal, vec3 ToLight, vec3 LightColor, float Fresnel, float Roughness, float MetallicColor)
{
	vec3 Diffuse;

	vec3 view = normalize(CameraPosition - WorldPos);

	float NdotL = max(0.0f, dot(normal, ToLight));
	float NdotV = max(0.0f, dot(normal, view));

	vec3 R =  -view + 2 * normal * dot(view,normal);

	
	vec3 F0 = mix(vec3(Fresnel), BaseColor, MetallicColor);
	vec3 F        = fresnelSchlickRoughness(NdotV, F0, Roughness);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - MetallicColor;

	vec3 irradiance = texture(u_IrradianceTexture, normal).rgb;
	Diffuse =  BaseColor * irradiance;


	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(u_PreFilterMap, R,  Roughness * MAX_REFLECTION_LOD).rgb;    

	vec3 Specular;
	vec2 envBRDF  = texture(u_BrdfLUT, vec2(NdotV, Roughness)).rg;
	Specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	FinalColor += (kD * Diffuse + Specular * LightColor) * NdotL;

	return FinalColor;
};



void main()
{
	float gamma = 2.2;
	

	vec4 cColor = vec4(0,0,0,1.0f);

	vec3 BaseColor = pow(gBaseColor, vec3(gamma));
	vec3 SpecularColor = gSpecularColor;
	float Fresnel = gFresnel;
	float Roughness = gRoughnessColor;
	float MetallicColor = gMetallicColor;

	MetallicColor = max(0.01f, MetallicColor);
	
	if((gTextureMask & MATERIAL_BASE_MAP) != 0){
		BaseColor = texture(u_BaseTexture, Texcoord0).xyz;
	}
	//BaseColor = vec3(Texcoord0.x,Texcoord0.y,0);
	//BaseColor.xyz *= 2;

	vec3 normalTBN = Normal;
	

	if((gTextureMask & MATERIAL_NORMAL_MAP) != 0){
		vec3 N = normalize(Normal);
		vec3 T = normalize(Tangent);
		vec3 B = normalize(Bitangent);

		mat3 TBN = mat3(T, B, N);

		vec3 normal = texture(u_NormalTexture, Texcoord0).xyz;
		normal = (2.0f * normal) - 1.0f;

		normalTBN =  TBN * normal;
	}

	if((gTextureMask & MATERIAL_ROUGHNESS_MAP) != 0){
		Roughness = texture(u_RoughnessTexture, Texcoord0).x;
	}

	if((gTextureMask & MATERIAL_METALLIC_MAP) != 0){
		MetallicColor = texture(u_MetallicTexture, Texcoord0).x;
	}


	BaseColor = pow(BaseColor, vec3(gamma) );


//		vec3 vToLight = -normalize(gMainLight.vec3Direction);
//		vec3 lightColor = gMainLight.vec3LightColor * 1.0f;
//		cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, SpecularColor, normalize(normalTBN), vToLight, lightColor, Fresnel, Roughness, MetallicColor);

	for(uint i = 0u; i < g_nLights;++i){
		
		uint lightType = g_lights[i].nLightType;
		vec3 vToLight;
		if(lightType == TYPE_LIGHT_DIRECTION)
			vToLight = -normalize(g_lights[i].vec3Direction);
		else if(lightType == TYPE_LIGHT_DIRECTION_BY_POSITION)
			vToLight = normalize(g_lights[i].vec3Position - WorldPos);

		vec3 lightColor = g_lights[i].vec3LightColor * 1.0f;
		cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, SpecularColor, normalize(normalTBN), vToLight, lightColor, Fresnel, Roughness, MetallicColor);
	}


	float S = 1.0;
	cColor.rgb = vec3(S * aces_approx(cColor.xyz * 0.8));
	
	cColor.rgb = pow(cColor.rgb, vec3(1.0 / gamma));
	
	FragColor = cColor;
	//FragColor = vec4(BaseColor, 1.0f);
}
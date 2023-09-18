#version 330 core

in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

const float c_PI = 3.141592;

struct Light
{
	vec3 vec3LightColor;
	float fIntensity;
	vec3 vec3Direction;
};

uniform Light gMainLight;	
uniform vec3 CameraPosition;

const float roughness = 0.5f;
const float Fresnel = 0.22f;
const float Metallic = 0.04f;
const vec3 SpecularColor = vec3(1,1,1);

float NDF_GGXTR(vec3 n, vec3 h, float a)
{
	float a_pow = pow(a, 2);
	return a_pow / (c_PI * pow(  pow(dot(n,h),2) * (a_pow - 1.f) + 1.f , 2) );

	float d = (dot(n,h) * a_pow - dot(n,h)) * dot(n,h) + 1;
	return a_pow / (c_PI * d * d);
}

float Fresnel_Cook_Torrance(vec3 h, vec3 v, float F0)
{
	float n = (1 + sqrt(F0)) / (1 - sqrt(F0));
	float c = dot(h, v);
	float g = sqrt( pow(n, 2) + pow(c , 2) -1 );
	return 0.5f * pow( (g - c) / (g + c), 2) * ( 1.0f + pow( ((g + c) * c - 1)  / ((g - c) * c + 1) , 2) );
}
vec3 FresnelSchlick(vec3 h, vec3 v, vec3 F0)
{
	return F0 + ( (1.f - F0 ) * pow( 1.f - (dot(h, v)) , 5 ) );
}

vec3 Unreal_F_Schlick(vec3 specularColor, vec3 h, vec3 v)
{
	float Fc = pow((1 - dot(v, h)), 5);

	return clamp( (50.0f * specularColor.g) * Fc + (1 - Fc) * specularColor, 0.0f, 1.0f);
}

float SchlickGGX(vec3 n, vec3 v, float a)
{
	float k = a * sqrt(2 / c_PI);
	return dot(n, v) / ( (dot(n,v) * ( 1.f - k )) + k );
}

vec3 Cook_Torrance_BRDF(vec3 FinalColor, vec3 BaseColor, vec3 normal, vec3 ToLight)
{
	vec3 Diffuse;

	float NdotL = dot(normal, ToLight);

	float Lambert = NdotL / c_PI;
	Diffuse = Lambert * BaseColor;

	FinalColor += Diffuse;

	return FinalColor;
};



void main()
{
	vec3 vToLight = -normalize(gMainLight.vec3Direction);

	vec4 cColor = vec4(0,0,0,1.0f);

	vec3 BaseColor = vec3(0,1,0);

	cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, Normal, vToLight);

	//cColor.rgb += vec3(0.2,0.2,0.2);

	
	FragColor = cColor;
}
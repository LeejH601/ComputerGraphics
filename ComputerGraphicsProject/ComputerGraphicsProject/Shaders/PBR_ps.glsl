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

const float roughness = 0.0f;
const float Fresnel = 0.22f;
const float Metallic = 0.04f;
const vec3 SpecularColor = vec3(1,1,1);

float NDF_GGXTR(vec3 n, vec3 h, float a)
{
	float a_pow = pow(a, 2);
	//return a_pow / (c_PI * pow(  pow(dot(n,h),2) * (a_pow - 1.f) + 1.f , 2) );

	float d = (dot(n,h) * a_pow - dot(n,h)) * dot(n,h) + 1;
	return a_pow / (c_PI * d * d);
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

float SchlickGGX(vec3 n, vec3 v, float k)
{
	return dot(n, v) / ( (dot(n,v) * ( 1.f - k )) + k );
}

vec3 Cook_Torrance_BRDF(vec3 FinalColor, vec3 BaseColor, vec3 normal, vec3 ToLight)
{
	vec3 halfwayVec = normalize(ToLight + normal);
	float D = NDF_GGXTR(normal,halfwayVec, roughness );

	vec3 view = normalize( CameraPosition - WorldPos);

	//vec3 F0 = mix( vec3(Fresnel), BaseColor, Metallic);
	vec3 F0 = vec3(pow((Fresnel - 1) / (Fresnel + 1), 2));

	//vec3 F = Unreal_F_Schlick(SpecularColor, halfwayVec, view);
	vec3 F = FresnelSchlick(halfwayVec, view, F0);

	float G = SchlickGGX(normal, view, roughness );



	//vec3 fLambert =  mix(vec3(1,1,1) - F, vec3(0,0,0), Metallic);
	float fLambert =  dot(normal,ToLight) / c_PI;

	vec3 Diffuse = fLambert * BaseColor;

	float w0 =  dot(normal, view);
	float wi = dot(normal, ToLight);
	vec3 Specular = (D * F * G) /(4 * w0 * wi);

	//Specular = mix(Specular, BaseColor, Metallic);

	FinalColor += (Diffuse + Specular) * wi;

	return FinalColor;
};



void main()
{
	//FragColor = vec4(WorldPos,1.0f);

	vec3 vToLight = -normalize(gMainLight.vec3Direction);

	vec4 cColor = vec4(0,0,0,1.0f);

	vec3 BaseColor = vec3(1,0,0);

	if(dot(vToLight, Normal) > 0.f)
		cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, Normal, vToLight);

	//cColor.rgb = (Normal + 1.0f) / 4.0f + 0.5f;
	//cColor.rgb = WorldPos / 2.0f + 0.5f;
	
	FragColor = cColor;
}
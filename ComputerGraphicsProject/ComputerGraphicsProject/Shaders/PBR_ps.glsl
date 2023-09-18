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

const float roughness = 0.1f;
const float Fresnel = 1.46f;
const float Metallic = 0.04f;
const vec3 SpecularColor = vec3(1,1,1);


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

vec3 Cook_Torrance_BRDF(vec3 FinalColor, vec3 BaseColor, vec3 normal, vec3 ToLight)
{
	vec3 Diffuse;

	vec3 view = normalize(CameraPosition - WorldPos);

	float NdotL = dot(normal, ToLight);
	float NdotV = max(0.00001f, dot(normal, view));

	//if(abs(NdotL) < 0.00001f)
		//NdotL -= 0.001f;

	float Lambert = max(NdotL / c_PI, 0.0f);
	Diffuse = Lambert * BaseColor;
	
	
	//NdotL = NdotL * 0.5f + 0.5f;


	vec3 halfv = normalize(ToLight + normal);

	float D = GGX_Trowbridge_Reitz(normal, halfv, roughness);
	float G = G_Schlick_GGX(NdotV, roughness);

	float F0 = pow( (Fresnel - 1) / (Fresnel + 1) ,2 );
	float F = F_Cook_Torrance(view, halfv, F0) * NdotL;

	float a = (4 * NdotL * NdotV);
	float Specular;
	
	
		Specular = (D * G * F) / a;
		//Specular = D * F * Vis_SmithJoint(pow(roughness,2),NdotV,NdotL );



	
	FinalColor += mix(Diffuse, vec3(Specular) * SpecularColor, Metallic) ;
	//FinalColor +=  Diffuse + vec3(Specular) * SpecularColor ;
	//FinalColor += Diffuse;

	FinalColor = max(vec3(0.0f), FinalColor);

	return FinalColor;
};



void main()
{
	vec3 vToLight = -normalize(gMainLight.vec3Direction);

	vec4 cColor = vec4(0,0,0,1.0f);

	vec3 BaseColor = vec3(0,1.0f,0);
	float gamma = 2.2;

	BaseColor.x = pow(BaseColor.x, gamma );
	BaseColor.y = pow(BaseColor.y , gamma );
	BaseColor.z = pow(BaseColor.z, gamma );

	cColor.rgb = Cook_Torrance_BRDF( cColor.rgb, BaseColor, normalize(Normal), vToLight);

	float S = 1.0;
	cColor.rgb = vec3(S * aces_approx(cColor.xyz * 0.8));
	//cColor.rgb += vec3(0.2,0.2,0.2);
	
	cColor.x = pow(cColor.x, 1.0 / gamma);
	cColor.y = pow(cColor.y, 1.0 / gamma);
	cColor.z = pow(cColor.z, 1.0 / gamma) ;
	
	FragColor = cColor;
}
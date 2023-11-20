#version 440 core

in vec3 WorldPos;
in vec3 Normal;
in vec2 Texcoord0;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 gBaseColor;

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

void main()
{		
    FragColor = vec4(gBaseColor, 1.0);

    float S = 1.0;
	FragColor.rgb = vec3(S * aces_approx(FragColor.xyz * 0.8));
}
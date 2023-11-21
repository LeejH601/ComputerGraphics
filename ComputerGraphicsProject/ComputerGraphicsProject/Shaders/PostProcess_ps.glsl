#version 440 core

out vec4 FragColor;

in vec3 localPos;
in vec2 texcoord;

uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;


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
    FragColor = vec4(texture(gColor, texcoord).rgb, 1.0);
}
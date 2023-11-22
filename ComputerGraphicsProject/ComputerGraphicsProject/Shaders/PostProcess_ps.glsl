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
    float gamma = 2.2;
    vec4 test1 = vec4(texture(gNormal, texcoord).rgb, 1.0);
    vec4 test2 = vec4(texture(gPosition, texcoord).rgb, 1.0);

    vec3 cColor = texture(gColor, texcoord).rgb;

    float S = 1.0;
	cColor.rgb = vec3(S * aces_approx(cColor.xyz * 0.8));
	
	cColor.rgb = pow(cColor.rgb, vec3(1.0 / gamma));

    FragColor = vec4(cColor, 1.0);
    //FragColor = test1;
}
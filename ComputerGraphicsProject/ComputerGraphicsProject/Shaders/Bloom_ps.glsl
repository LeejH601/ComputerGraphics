#version 330 core
out vec4 FragColor;

in vec3 localPos;
in vec2 texcoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform float bloomStrength = 0.08f;

void main()
{	
    vec3 cColor = vec3(0.0);
    
    vec3 hdrColor = texture(scene, texcoord).rgb;
    vec3 bloomColor = texture(bloomBlur, texcoord).rgb;
    cColor = mix(hdrColor, bloomColor, bloomStrength);

    FragColor = vec4(cColor, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube u_BaseTexture;

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
    vec3 envColor = textureLod(u_BaseTexture, localPos, 0.0).rgb;
    
    envColor = aces_approx(envColor);
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
}
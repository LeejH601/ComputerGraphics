#version 330 core
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube u_BaseTexture; // environmentMap

const float c_PI = 3.14159265359;

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);

}

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
  
  
    FragColor = vec4(irradiance, 1.0);
}
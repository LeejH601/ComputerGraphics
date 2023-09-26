#version 330 core
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube u_BaseTexture; // environmentMap

const float c_PI = 3.14159265359;

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
    vec3 normal = normalize(localPos);
    
    vec3 irradiance = vec3(0.0f);
    
    float gamma = 2.2f;

    vec3 up = vec3(0.0f,1.0f,0.0f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.008f;
    int nSamples = 0;

    for(float phi = 0.0f; phi < 2.0 * c_PI; phi += sampleDelta)
    {
        for(float theta = 0.0f; theta < 0.5f * c_PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(u_BaseTexture, sampleVec).rgb * cos(theta) * sin(theta);
            nSamples++;
        }
    }
    //irradiance = pow(irradiance, vec3(gamma));
    irradiance = c_PI * irradiance * (1.0f / float(nSamples));
    //irradiance = pow(irradiance, vec3(1.0f / gamma));
  
    FragColor = vec4(irradiance, 1.0);
    //FragColor = vec4(localPos, 1.0);
}
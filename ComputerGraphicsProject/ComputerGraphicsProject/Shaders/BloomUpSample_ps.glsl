#version 330 core
out vec4 FragColor;
in vec3 localPos;
in vec2 texcoord;

uniform sampler2D srcTexture;
uniform float filterRadius;

void main()
{	
    float x = filterRadius;
    float y = filterRadius;

    vec3 a = texture(srcTexture, vec2(texcoord.x - x, texcoord.y + y)).rgb;
    vec3 b = texture(srcTexture, vec2(texcoord.x,     texcoord.y + y)).rgb;
    vec3 c = texture(srcTexture, vec2(texcoord.x + x, texcoord.y + y)).rgb;

    vec3 d = texture(srcTexture, vec2(texcoord.x - x, texcoord.y)).rgb;
    vec3 e = texture(srcTexture, vec2(texcoord.x,     texcoord.y)).rgb;
    vec3 f = texture(srcTexture, vec2(texcoord.x + x, texcoord.y)).rgb;

    vec3 g = texture(srcTexture, vec2(texcoord.x - x, texcoord.y - y)).rgb;
    vec3 h = texture(srcTexture, vec2(texcoord.x,     texcoord.y - y)).rgb;
    vec3 i = texture(srcTexture, vec2(texcoord.x + x, texcoord.y - y)).rgb;

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    FragColor.rgb = e*4.0;
    FragColor.rgb += (b+d+f+h)*2.0;
    FragColor.rgb += (a+c+g+i);
    FragColor.rgb *= 1.0 / 16.0;
    FragColor.w = 1.0f;
}
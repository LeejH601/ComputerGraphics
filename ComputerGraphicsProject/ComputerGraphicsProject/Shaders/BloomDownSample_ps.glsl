#version 330 core
out vec4 FragColor;

in vec3 localPos;
in vec2 texcoord;

uniform sampler2D srcTexture;
uniform vec2 srcResolution;

void main()
{	
    vec2 srcTexelSize = 1.0 / srcResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(srcTexture, vec2(texcoord.x - 2*x, texcoord.y + 2*y)).rgb;
    vec3 b = texture(srcTexture, vec2(texcoord.x,       texcoord.y + 2*y)).rgb;
    vec3 c = texture(srcTexture, vec2(texcoord.x + 2*x, texcoord.y + 2*y)).rgb;

    vec3 d = texture(srcTexture, vec2(texcoord.x - 2*x, texcoord.y)).rgb;
    vec3 e = texture(srcTexture, vec2(texcoord.x,       texcoord.y)).rgb;
    vec3 f = texture(srcTexture, vec2(texcoord.x + 2*x, texcoord.y)).rgb;

    vec3 g = texture(srcTexture, vec2(texcoord.x - 2*x, texcoord.y - 2*y)).rgb;
    vec3 h = texture(srcTexture, vec2(texcoord.x,       texcoord.y - 2*y)).rgb;
    vec3 i = texture(srcTexture, vec2(texcoord.x + 2*x, texcoord.y - 2*y)).rgb;

    vec3 j = texture(srcTexture, vec2(texcoord.x - x, texcoord.y + y)).rgb;
    vec3 k = texture(srcTexture, vec2(texcoord.x + x, texcoord.y + y)).rgb;
    vec3 l = texture(srcTexture, vec2(texcoord.x - x, texcoord.y - y)).rgb;
    vec3 m = texture(srcTexture, vec2(texcoord.x + x, texcoord.y - y)).rgb;


    FragColor.rgb = e*0.125;
    FragColor.rgb += (a+c+g+i)*0.03125;
    FragColor.rgb += (b+d+f+h)*0.0625;
    FragColor.rgb += (j+k+l+m)*0.125;
    FragColor.a = 1.0f;
}
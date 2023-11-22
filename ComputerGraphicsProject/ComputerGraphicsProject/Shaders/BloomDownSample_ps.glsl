#version 330 core
out vec4 FragColor;

in vec3 localPos;
in vec2 texcoord;

uniform sampler2D srcTexture;
uniform vec2 srcResolution;
uniform int mipLevel;

vec3 PowVec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float invGamma = 1.0 / 2.2;
vec3 ToSRGB(vec3 v) { return PowVec3(v, invGamma); }

float RGBToLuminance(vec3 col)
{
    return dot(col, vec3(0.2126f, 0.7152f, 0.0722f));
}

float KarisAverage(vec3 col)
{
    // Formula is 1 / (1 + luma)
    float luma = RGBToLuminance(ToSRGB(col)) * 0.25f;
    return 1.0f / (1.0f + luma);
}

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


    vec3 groups[5];
    switch (mipLevel)
    {
        case 0:
            groups[0] = (a+b+d+e) * (0.125f/4.0f);
            groups[1] = (b+c+e+f) * (0.125f/4.0f);
            groups[2] = (d+e+g+h) * (0.125f/4.0f);
            groups[3] = (e+f+h+i) * (0.125f/4.0f);
            groups[4] = (j+k+l+m) * (0.5f/4.0f);
            groups[0] *= KarisAverage(groups[0]);
            groups[1] *= KarisAverage(groups[1]);
            groups[2] *= KarisAverage(groups[2]);
            groups[3] *= KarisAverage(groups[3]);
            groups[4] *= KarisAverage(groups[4]);
            FragColor.rgb = groups[0]+groups[1]+groups[2]+groups[3]+groups[4];
            break;
        default:
            FragColor.rgb = e*0.125;
            FragColor.rgb += (a+c+g+i)*0.03125;
            FragColor.rgb += (b+d+f+h)*0.0625;
            FragColor.rgb += (j+k+l+m)*0.125;
            break;
    }
  
    FragColor.rgb = max(FragColor.rgb, 0.0001f);

    FragColor.a = 1.0f;
}
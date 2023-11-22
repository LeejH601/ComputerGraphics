#version 330 core

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_texcoord0;
in vec3 v_Tangent;
in vec3 v_Bitangent;

out vec3 localPos;
out vec2 texcoord;


void main()
{
    gl_Position = vec4(v_Position.xy, 0.0f, 1.0f);
    texcoord = v_texcoord0;
}
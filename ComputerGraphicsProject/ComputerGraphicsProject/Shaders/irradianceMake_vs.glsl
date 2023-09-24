#version 330 core
layout (location = 0) in vec3 v_Position;

out vec3 localPos;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;

void main()
{
    localPos = v_Position;

    mat4 rotView = mat4(mat3(viewTransform)); 
    vec4 clipPos = projectionTransform * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}
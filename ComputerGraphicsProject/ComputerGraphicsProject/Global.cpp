#include "Global.h"


glm::quat IObject::GetQauternion()
{
    return glm::quat();
}

void IObject::SetQauternion(glm::quat rotation)
{
}

glm::mat4x4 viewPortMatrix = glm::mat4x4(
    0.5f * g_WindowSizeX,                     0.0f, 0.0f, 0.0f,
    0.0f,     -0.5f * g_WindowSizeY, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0 - 0.0, 0.0f,
    0.5f * g_WindowSizeX,      0.5f * g_WindowSizeY, 0.0f, 1.0f
);
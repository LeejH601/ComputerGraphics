#pragma once
#include "stdafx.h"

class CLight
{
public:
	glm::vec3 m_vec3LightColor = glm::vec3(1,0.8,1.2);
	glm::f32 m_fIntensity = 1.0f;
	glm::vec3 m_vec3Direction;
	UINT m_LightType = 0;

	CLight() = default;
};
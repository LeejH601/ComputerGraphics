#pragma once
#include "stdafx.h"

class CLight
{
public:
	glm::vec3 m_vec3LightColor = glm::vec3(1,0.8,1);
	glm::f32 m_fIntensity = 0.0f;
	glm::vec3 m_vec3Direction;
	UINT m_LightType = 0;

	CLight() = default;
	virtual ~CLight() = default;

	virtual void CreateShaderVariables();
	virtual void BindShaderVariables(GLuint s_Program);
};


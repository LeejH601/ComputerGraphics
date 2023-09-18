#pragma once
#include "stdafx.h"

class CLight
{
public:
	glm::vec3 m_vec3LightColor;
	glm::f32 m_fIntensity;
	glm::vec3 m_vec3Direction;

	CLight() = default;
	virtual ~CLight() = default;

	virtual void CreateShaderVariables();
	virtual void BindShaderVariables(GLuint s_Program);
};


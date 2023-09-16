#pragma once
#include "stdafx.h"

class CCamera
{
private:
	glm::vec4 m_f4Rotation;
	glm::vec3 m_f3Position;

	glm::vec3 m_f3Look;
	glm::vec3 m_f3Up;
	glm::vec3 m_f3Right;

public:
	glm::mat4x4 m_mat4x4Projection;
	glm::mat4x4 m_mat4x4View;

	glm::vec4 m_f4ViewPort;

public:
	CCamera();
	virtual ~CCamera();

	void RegenarationViewMatrix();
	void GenerateProjectionMatrix(float fov, float aspect, float zNear, float zFar);

	virtual void BindShaderVariables(GLuint s_Program);
};


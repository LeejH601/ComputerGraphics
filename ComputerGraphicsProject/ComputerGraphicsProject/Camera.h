#pragma once
#include "stdafx.h"

class CCamera
{
private:
	glm::vec4 m_vec4Rotation;
	glm::vec3 m_vec3Position;

	glm::vec3 m_vec3Look;
	glm::vec3 m_vec3Up;
	glm::vec3 m_vec3Right;

public:
	glm::mat4x4 m_mat4x4Projection;
	glm::mat4x4 m_mat4x4View;

	glm::vec4 m_vec4ViewPort;

public:
	CCamera();
	virtual ~CCamera();

	void RegenarationViewMatrix();
	void GenerateProjectionMatrix(float fov, float aspect, float zNear, float zFar);

	virtual void BindShaderVariables(GLuint s_Program);

	glm::vec3 GetPosition() { return m_vec3Position; };
	void SetPosision(glm::vec3 pos) { m_vec3Position = pos; };
};


#include "Camera.h"

CCamera::CCamera()
{
	m_f3Position = { 0,-5,0 };
	m_f3Look = { 0,1,0 };
	m_f3Up = { 0,0,1 };
	m_f3Right = { 1,0,0 };
}

CCamera::~CCamera()
{

}

void CCamera::RegenarationViewMatrix()
{
	m_mat4x4View = glm::lookAt(m_f3Position, m_f3Look, m_f3Up);
}

void CCamera::GenerateProjectionMatrix(float fov, float aspect, float zNear, float zFar)
{
	m_mat4x4Projection = glm::perspective(fov, aspect, zNear, zFar);
	m_mat4x4Projection = glm::translate(m_mat4x4Projection, glm::vec3(0.0, 0.0, -5.0));
}

void CCamera::BindShaderVariables(GLuint s_Program)
{
	if (s_Program == -1)
		return;
	RegenarationViewMatrix();

	unsigned int viewLocation;
	unsigned int projectionLocation;

	viewLocation = glGetUniformLocation(s_Program, "gmtxVeiw");	// ViewMatrix
	projectionLocation = glGetUniformLocation(s_Program, "gmtxProj");	// ProjMatrix

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &m_mat4x4View[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &m_mat4x4Projection[0][0]);
}

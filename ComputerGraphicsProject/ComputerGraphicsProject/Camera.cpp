#include "Camera.h"

CCamera::CCamera()
{
	m_f3Position = { 0,0,0 };
	m_f3Look = { 0,0,-1 };
	m_f3Up = { 0,1, 0 };
	m_f3Right = { 1,0,0 };

	m_f4ViewPort = { 0,0,1280,768 };
}

CCamera::~CCamera()
{

}

void CCamera::RegenarationViewMatrix()
{
	m_f3Look = glm::normalize(m_f3Look);
	m_f3Right = glm::cross(m_f3Up, m_f3Look);
	m_f3Up = glm::cross(m_f3Look, m_f3Right);

	m_mat4x4View = {
		m_f3Right.x, m_f3Up.x, m_f3Look.x, 0,
		m_f3Right.y, m_f3Up.y, m_f3Look.y, 0,
		m_f3Right.z, m_f3Up.z, m_f3Look.z, 0,
		glm::dot(m_f3Position, m_f3Right), -glm::dot(m_f3Position, m_f3Up),glm::dot(m_f3Position, m_f3Look),1
	};

	/*m_mat4x4View[0][0] = m_f3Right.x; m_mat4x4View[0][1] = m_f3Up.x; m_mat4x4View[0][2] = m_f3Look.x;
	m_mat4x4View[1][0] = m_f3Right.y; m_mat4x4View[1][1] = m_f3Up.y; m_mat4x4View[1][2] = m_f3Look.y;
	m_mat4x4View[2][0] = m_f3Right.z; m_mat4x4View[2][1] = m_f3Up.z; m_mat4x4View[2][2] = m_f3Look.z;
	m_mat4x4View[3][0] = -glm::dot(m_f3Position, m_f3Right);
	m_mat4x4View[3][1] = -glm::dot(m_f3Position, m_f3Up);
	m_mat4x4View[3][2] = -glm::dot(m_f3Position, m_f3Look);*/

	//m_mat4x4View = glm::lookAt(m_f3Position, m_f3Look, m_f3Up);
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
	glViewport(m_f4ViewPort.x, m_f4ViewPort.y, m_f4ViewPort.z, m_f4ViewPort.w);

	RegenarationViewMatrix();

	GLuint viewLocation;
	GLuint projectionLocation;

	viewLocation = glGetUniformLocation(s_Program, "viewTransform");	// ViewMatrix
	projectionLocation = glGetUniformLocation(s_Program, "projectionTransform");	// ProjMatrix

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &m_mat4x4View[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &m_mat4x4Projection[0][0]);
}

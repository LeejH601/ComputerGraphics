#include "Camera.h"

CCamera::CCamera()
{
	m_vec3Position = { 0,0, 0.5 };
	m_vec3Look = { 0,0,1 };
	m_vec3Up = { 0,1, 0 };
	m_vec3Right = { -1,0,0 };

	m_vec4ViewPort = { 0,0,1280,768 };
}

CCamera::~CCamera()
{

}

void CCamera::RegenarationViewMatrix()
{
	m_vec3Look = glm::normalize(m_vec3Look);
	m_vec3Right = glm::cross(m_vec3Up, m_vec3Look);
	m_vec3Up = glm::cross(m_vec3Look, m_vec3Right);

	m_mat4x4View = {
		m_vec3Right.x, m_vec3Up.x, m_vec3Look.x, 0,
		m_vec3Right.y, m_vec3Up.y, m_vec3Look.y, 0,
		m_vec3Right.z, m_vec3Up.z, m_vec3Look.z, 0,
		-glm::dot(m_vec3Position, m_vec3Right), -glm::dot(m_vec3Position, m_vec3Up), -glm::dot(m_vec3Position, m_vec3Look),1
	};

	/*m_mat4x4View[0][0] = m_vec3Right.x; m_mat4x4View[0][1] = m_vec3Up.x; m_mat4x4View[0][2] = m_vec3Look.x;
	m_mat4x4View[1][0] = m_vec3Right.y; m_mat4x4View[1][1] = m_vec3Up.y; m_mat4x4View[1][2] = m_vec3Look.y;
	m_mat4x4View[2][0] = m_vec3Right.z; m_mat4x4View[2][1] = m_vec3Up.z; m_mat4x4View[2][2] = m_vec3Look.z;
	m_mat4x4View[3][0] = -glm::dot(m_vec3Position, m_vec3Right);
	m_mat4x4View[3][1] = -glm::dot(m_vec3Position, m_vec3Up);
	m_mat4x4View[3][2] = -glm::dot(m_vec3Position, m_vec3Look);*/

	//m_mat4x4View = glm::lookAt(m_vec3Position, m_vec3Look, m_vec3Up);
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
	glViewport(m_vec4ViewPort.x, m_vec4ViewPort.y, m_vec4ViewPort.z, m_vec4ViewPort.w);

	RegenarationViewMatrix();

	GLuint viewLocation;
	GLuint projectionLocation;
	GLuint PosLocation;

	viewLocation = glGetUniformLocation(s_Program, "viewTransform");	// ViewMatrix
	projectionLocation = glGetUniformLocation(s_Program, "projectionTransform");	// ProjMatrix
	PosLocation = glGetUniformLocation(s_Program, "CameraPosition");	// CameraPos

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &m_mat4x4View[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &m_mat4x4Projection[0][0]);
	glUniformMatrix3fv(PosLocation, 1, GL_FALSE, glm::value_ptr(m_vec3Position));
}

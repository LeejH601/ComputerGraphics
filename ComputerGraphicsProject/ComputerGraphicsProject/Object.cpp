#include "Object.h"
#include "Mesh.h"

CObject::CObject()
{
	m_mat4x4Transform = glm::identity<glm::mat4x4>();
	m_mat4x4Wolrd = glm::identity<glm::mat4x4>();
	m_f3Position = { 0,0,0 };

	m_f4Rotation = glm::vec4(0, 0, 0, 1);
}

CObject::CObject(const CObject& other)
{
	m_mat4x4Transform = other.m_mat4x4Transform;
	m_mat4x4Wolrd = other.m_mat4x4Wolrd;

	m_f4Rotation = other.m_f4Rotation;

}

CObject::CObject(CObject&& other) noexcept
{
	m_mat4x4Transform = other.m_mat4x4Transform;
	m_mat4x4Wolrd = other.m_mat4x4Wolrd;

	m_f4Rotation = other.m_f4Rotation;
	
	// 할당 해제
	other.m_mat4x4Transform = glm::identity<glm::mat4x4>();
	other.m_mat4x4Wolrd = glm::identity<glm::mat4x4>();

	other.m_f4Rotation = glm::vec4(0, 0, 0, 1);
}

CObject& CObject::operator=(const CObject& other)
{
	m_mat4x4Transform = other.m_mat4x4Transform;
	m_mat4x4Wolrd = other.m_mat4x4Wolrd;

	m_f4Rotation = other.m_f4Rotation;

	return *this;
}

CObject& CObject::operator=(CObject&& other) noexcept
{
	m_mat4x4Transform = other.m_mat4x4Transform;
	m_mat4x4Wolrd = other.m_mat4x4Wolrd;

	m_f4Rotation = other.m_f4Rotation;

	// 할당 해제
	other.m_mat4x4Transform = glm::identity<glm::mat4x4>();
	other.m_mat4x4Wolrd = glm::identity<glm::mat4x4>();

	other.m_f4Rotation = glm::vec4(0, 0, 0, 1);

	return *this;
}

void CObject::Render()
{
	if (m_pMesh)
		m_pMesh->Render();
}

void CObject::BindShaderVariables(GLuint s_Program)
{
	GLuint worldLoc = glGetUniformLocation(s_Program, "gmtxWorld");
	glEnableVertexAttribArray(worldLoc);

	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m_mat4x4Wolrd));

	if (m_pMesh) {
		m_pMesh->BindShaderVariables(s_Program);
	}
}

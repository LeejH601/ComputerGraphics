#include "Object.h"

CObject::CObject()
{
	m_mat4x4Transform = glm::identity<glm::mat4x4>();
	m_mat4x4Wolrd = glm::identity<glm::mat4x4>();

	m_f4Rotation = glm::identity<glm::vec4>();
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

	other.m_f4Rotation = glm::identity<glm::vec4>();
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

	other.m_f4Rotation = glm::identity<glm::vec4>();

	return *this;
}

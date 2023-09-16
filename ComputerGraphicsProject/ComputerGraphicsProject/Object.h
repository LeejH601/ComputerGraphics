#pragma once
#include <string>
#include <vector>
#include "stdafx.h"
#include "Material.h"
#include <memory>

class CMesh;
class CObject
{
	glm::mat4x4 m_mat4x4Transform;
	glm::mat4x4 m_mat4x4Wolrd;

	glm::vec4 m_f4Rotation;
	glm::vec3 m_f3Position;
	glm::vec3 m_f3Scale;

	CMesh* m_pMesh = nullptr;

	std::string m_strObjectName;
	
	UINT m_nMaterials;
	std::vector<std::shared_ptr<CMaterial>> m_ppMaterials;



public:
	CObject();
	virtual ~CObject() = default;
	CObject(const CObject& other);
	CObject(CObject&& other) noexcept;
	CObject& operator=(const CObject& other);
	CObject& operator=(CObject&& other) noexcept;


};


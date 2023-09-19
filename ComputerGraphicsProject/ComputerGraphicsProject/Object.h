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

	glm::vec4 m_vec4Rotation;
	glm::vec3 m_vec3Position;
	glm::vec3 m_vec3Scale;

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

	virtual void Render();

	virtual void BindShaderVariables(GLuint s_Program);

	void SetMesh(CMesh* mesh) { m_pMesh = mesh; };
	void SetMaterial(std::shared_ptr<CMaterial>& pMaterial);
};


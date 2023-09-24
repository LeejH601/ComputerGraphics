#pragma once
#include <string>
#include <vector>
#include "stdafx.h"
#include "Material.h"
#include "Global.h"
#include <memory>



class CMesh;
class CObject
{
	glm::mat4x4 m_mat4x4Transform;
	glm::mat4x4 m_mat4x4Wolrd;

	glm::vec4 m_vec4Rotation;
	glm::vec3 m_vec3Position;
	glm::vec3 m_vec3Scale;

	std::shared_ptr<CMesh> m_pMesh = nullptr;

	std::string m_strObjectName;
	
	UINT m_nMaterials;
	std::vector<std::shared_ptr<CMaterial>> m_ppMaterials;

	std::shared_ptr<CObject> m_pChild = nullptr;
	CObject* m_pParent = nullptr;
	std::shared_ptr<CObject> m_pSibling = nullptr;

public:
	CObject();
	virtual ~CObject() = default;
	CObject(const CObject& other);
	CObject(CObject&& other) noexcept;
	CObject& operator=(const CObject& other);
	CObject& operator=(CObject&& other) noexcept;

	virtual void Render();

	virtual void BindShaderVariables(GLuint s_Program);

	void SetMesh(std::shared_ptr<CMesh>& mesh) { m_pMesh = mesh; };
	void SetMaterial(std::shared_ptr<CMaterial>& pMaterial);
	void SetMaterial(int nMaterial, std::shared_ptr<CMaterial> pMaterial);
	void SetChild(std::shared_ptr<CObject> child);

	void SetPosition(glm::vec3 position) { m_vec3Position = position; };
	glm::vec3 GetPosition() { return m_vec3Position; };
	void UpdateTransform(glm::mat4x4* parent);

	void LoadFrameHierarchyFromFile(CObject* pParent, FILE* pInFile, int* pnSkinnedMeshes = nullptr);
	void LoadMaterialsFromFile(CObject* pParent, FILE* pInFile);
	void LoadGeometryAndAnimationFromFile(const char* pstrFileName);

	std::shared_ptr<CMaterial> GetMaterial(int index) {
		if (index < m_nMaterials)
			return m_ppMaterials[index];
		return nullptr;
	}
};

class IMoveContext
{
protected:
	IObject* m_pObject;
public:
	IMoveContext();
	virtual ~IMoveContext();

	virtual void SetTarget(IObject* pObject) { m_pObject = pObject; };
	virtual void Rotate(glm::vec4& Quaternion, float angle, glm::vec3 axis);
};

class CRotateContext : public IMoveContext
{
public:
	virtual void Rotate(glm::vec4& Quaternion, float angle, glm::vec3 axis);
};

class CLoadedModelInfo
{
public:
	CLoadedModelInfo() { }
	~CLoadedModelInfo();

	std::string pFilePath;
	std::shared_ptr<CObject> m_pModelRootObject = NULL;

	//int m_nSkinnedMeshes = 0;
	//std::vector<CSkinnedMesh*> m_ppSkinnedMeshes; //[SkinnedMeshes], Skinned Mesh Cache

	//std::shared_ptr<CAnimationSets> m_pAnimationSets = NULL;
public:
	//void PrepareSkinning();

};
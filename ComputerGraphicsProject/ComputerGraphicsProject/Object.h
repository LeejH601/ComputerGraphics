#pragma once
#include <string>
#include <vector>
#include "stdafx.h"
#include "Material.h"
#include "Global.h"
#include <memory>



class CMesh;
class CSkinnedMesh;
class CAnimationSets;
class CAnimationController;
class CObject
{
public:
	glm::mat4x4 m_mat4x4Transform;
	glm::mat4x4 m_mat4x4Wolrd;

protected:
	glm::quat m_vec4Rotation{ 1,0,0,0 };
	glm::vec3 m_vec3Position{ 0,0,0 };
	glm::vec3 m_vec3Scale{ 1,1,1 };

	std::shared_ptr<CMesh> m_pMesh = nullptr;

	std::string m_strObjectName;
	
	UINT m_nMaterials;
	std::vector<std::shared_ptr<CMaterial>> m_ppMaterials;

	std::shared_ptr<CObject> m_pChild = nullptr;
	CObject* m_pParent = nullptr;
	std::shared_ptr<CObject> m_pSibling = nullptr;

	DirectX::BoundingOrientedBox m_OBB;

	std::unique_ptr<CAnimationController> m_pSkinnedAnimationController;

public:
	CObject();
	virtual ~CObject() = default;
	/*CObject(const CObject& other);
	CObject(CObject&& other) noexcept;
	CObject& operator=(const CObject& other);
	CObject& operator=(CObject&& other) noexcept;*/

	void SetName(std::string str) { m_strObjectName = str; };
	std::string GetName() { return m_strObjectName; };
	glm::quat GetRotation() { return m_vec4Rotation; };
	glm::vec3 GetScale() { return m_vec3Scale; };

	static CObject* FindFrameByName(CObject* object, std::string& name);

	virtual void Update(float fElapsedTime);
	virtual void Render(GLuint s_Program);

	virtual void BindShaderVariables(GLuint s_Program);

	void SetMesh(std::shared_ptr<CMesh>& mesh) { m_pMesh = mesh; };
	CMesh* GetMesh() { if (m_pMesh) return m_pMesh.get(); };
	std::shared_ptr<CMesh> GetMeshByShared() { return m_pMesh; };
	void SetMaterial(std::shared_ptr<CMaterial> pMaterial);
	void SetMaterial(int nMaterial, std::shared_ptr<CMaterial> pMaterial);
	void SetChild(std::shared_ptr<CObject> child);

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition() { return m_vec3Position; };
	void SetScale(glm::vec3 scale);
	void SetRotate(glm::quat rotate) { m_vec4Rotation = rotate; };
	void RotationQuat(float radian, glm::vec3 axis);
	void ReganerateTransform();
	void UpdateTransform(glm::mat4x4* parent = nullptr);


	void LoadFrameHierarchyFromFile(CObject* pParent, FILE* pInFile, int* pnSkinnedMeshes = nullptr);
	void LoadMaterialsFromFile(CObject* pParent, FILE* pInFile);
	void LoadGeometryAndAnimationFromFile(const char* pstrFileName);

	void FindAndSetSkinnedMesh(CSkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh);

	std::shared_ptr<CMaterial> GetMaterial(int index) {
		if (index < m_nMaterials)
			return m_ppMaterials[index];
		return nullptr;
	}
	std::vector<std::shared_ptr<CMaterial>>& GetAllMaterials() {
		return m_ppMaterials;
	}

	CObject* GetChild() { return m_pChild.get(); };
	CObject* GetSibling() { return m_pSibling.get(); };


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

	int m_nSkinnedMeshes = 0;
	std::vector<CSkinnedMesh*> m_ppSkinnedMeshes; //[SkinnedMeshes], Skinned Mesh Cache

	std::shared_ptr<CAnimationSets> m_pAnimationSets = NULL;
public:
	void PrepareSkinning();

};
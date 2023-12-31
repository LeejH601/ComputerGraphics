#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include <map>
#include <unordered_map>

class CResourceManager
{
private:
	std::vector<std::shared_ptr<CMaterial>> m_pMaterials;
	std::vector<std::shared_ptr<CMesh>> m_pMeshs;
	std::vector<std::shared_ptr<CTexture>> m_pTextures;

	std::unordered_map<std::string, UINT> m_MaterialNameMap;
	std::unordered_map<std::string, UINT> m_MeshNameMap;
	std::unordered_map<std::string, UINT> m_TextureNameMap;

	CCamera m_MeshViewCamera;
	std::vector<std::shared_ptr<CViewerTexture>> m_pMeshViewerTextures;
	std::unordered_map<std::string, UINT> m_ViewTextureNameMap;

	bool m_bBakedMeshViewTextures = false;

public:
	DECLARE_SINGLE(CResourceManager);
	CResourceManager();
	virtual ~CResourceManager();

	UINT RegisterMaterial(std::shared_ptr<CMaterial> material); 
	UINT RegisterMesh(std::shared_ptr<CMesh> mesh); 
	UINT RegisterTexture(std::shared_ptr<CTexture> texture); 
	std::shared_ptr<CMaterial> GetMaterialFromIndex(UINT index);
	std::shared_ptr<CMesh> GetMeshFromIndex(UINT index);
	std::shared_ptr<CTexture> GetTextureFromIndex(UINT index); 

	UINT GetTextureIndex(std::string str);
	UINT GetMaterialIndex(std::string str);
	UINT GetMeshIndex(std::string str);

	std::shared_ptr<CMaterial> GetMaterialFromName(std::string str);
	std::shared_ptr<CMesh> GetMeshFromName(std::string str);
	std::shared_ptr<CTexture> GetTextureFromName(std::string str);

	std::vector<std::shared_ptr<CTexture>>& GetTextureList() { return m_pTextures; };
	std::vector<std::shared_ptr<CMaterial>>& GetMaterialList() { return m_pMaterials; };
	std::vector<std::shared_ptr<CMesh>>& GetMeshList() { return m_pMeshs; };
	std::vector<std::shared_ptr<CViewerTexture>>& GetViewTextureList() { 
		if (m_bBakedMeshViewTextures == false)
			BakeViewTextures();
		return m_pMeshViewerTextures; 
	};

	bool SwapTexture(UINT t1_index, UINT t2_index);
	bool SwapMaterial(UINT t1_index, UINT t2_index);
	bool SwapMesh(UINT t1_index, UINT t2_index);

	std::shared_ptr<CTexture> ImportTexture(std::string path, GLuint samplingMethod);
	std::shared_ptr<CTexture> ImportTexture(std::string path, std::string textureName, GLuint samplingMethod);

	std::vector<std::string> GetTextureNameList();
	std::vector<std::string> GetMaterialNameList();
	std::vector<std::string> GetMeshNameList();

	void BakeViewTextures();
};


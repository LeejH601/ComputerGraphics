#pragma once
#include "stdafx.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class CResourceManager
{
private:
	std::vector<std::shared_ptr<CMaterial>> m_pMaterials;
	std::vector<std::shared_ptr<CMesh>> m_pMeshs;
	std::vector<std::shared_ptr<CTexture>> m_pTextures;

public:
	DECLARE_SINGLE(CResourceManager);
	CResourceManager();
	virtual ~CResourceManager();

	UINT RegisterMaterial(std::shared_ptr<CMaterial> material) { m_pMaterials.push_back(material); return m_pMaterials.end() - m_pMaterials.begin(); };
	UINT RegisterMesh(std::shared_ptr<CMesh> mesh) { m_pMeshs.push_back(mesh); return m_pMeshs.end() - m_pMeshs.begin(); };
	UINT RegisterTexture(std::shared_ptr<CTexture> mesh) { m_pTextures.push_back(mesh); return m_pTextures.end() - m_pTextures.begin(); };
	std::shared_ptr<CMaterial> GetMaterialFromIndex(UINT index) { if (index < m_pMaterials.size()) return m_pMaterials[index]; return nullptr; };
	std::shared_ptr<CMesh> GetMeshFromIndex(UINT index) { if (index < m_pMeshs.size()) return m_pMeshs[index]; return nullptr; };
	std::shared_ptr<CTexture> GetTextureFromIndex(UINT index) { if (index < m_pTextures.size()) return m_pTextures[index]; return nullptr; };


};


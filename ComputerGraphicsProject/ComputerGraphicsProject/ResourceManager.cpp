#include "ResourceManager.h"
#include <string_view>

using namespace std::literals;

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

UINT CResourceManager::RegisterMaterial(std::shared_ptr<CMaterial> material)
{
    if (material->GetName().length() == 0) {
        material->SetName(std::to_string(m_pMaterials.size()));
    }
    if (m_MaterialNameMap.find(material->GetName()) != m_MaterialNameMap.end())
        return -1;

    m_pMaterials.push_back(material); 
    m_MaterialNameMap.emplace(std::make_pair(material->GetName(), m_pMaterials.size() - 1));
    return m_pMaterials.end() - m_pMaterials.begin();
}

UINT CResourceManager::RegisterMesh(std::shared_ptr<CMesh> mesh)
{
    if (m_MeshNameMap.find(mesh->GetName()) != m_MeshNameMap.end())
        return -1;

    m_pMeshs.push_back(mesh); 
    m_MeshNameMap.emplace(std::make_pair(mesh->GetName(), m_pMeshs.size() - 1));
    return m_pMeshs.end() - m_pMeshs.begin();
}

UINT CResourceManager::RegisterTexture(std::shared_ptr<CTexture> texture)
{
    if (m_TextureNameMap.find(texture->GetName()) != m_TextureNameMap.end())
        return -1;

    m_pTextures.push_back(texture);
    m_TextureNameMap.emplace(std::make_pair(texture->GetName(), m_pTextures.size() - 1));
    return m_pTextures.end() - m_pTextures.begin();
}



std::shared_ptr<CMaterial> CResourceManager::GetMaterialFromIndex(UINT index)
{
    if (index < m_pMaterials.size()) 
        return m_pMaterials[index]; return nullptr;
}

std::shared_ptr<CMesh> CResourceManager::GetMeshFromIndex(UINT index)
{
    if (index < m_pMeshs.size()) 
        return m_pMeshs[index]; return nullptr;
}

std::shared_ptr<CTexture> CResourceManager::GetTextureFromIndex(UINT index)
{
    if (index < m_pTextures.size()) 
        return m_pTextures[index]; return nullptr;
}

UINT CResourceManager::GetTextureIndex(std::string str)
{
    auto iTexture = m_TextureNameMap.find(str);
    if (iTexture != m_TextureNameMap.end())
        return iTexture->second;
    return -1;
}

std::shared_ptr<CMaterial> CResourceManager::GetMaterialFromName(std::string str)
{
    auto iMaterial = m_MaterialNameMap.find(str);
    if (iMaterial != m_MaterialNameMap.end())
        return m_pMaterials[iMaterial->second];
    return nullptr;
}

std::shared_ptr<CMesh> CResourceManager::GetMeshFromName(std::string str)
{
    auto iMesh = m_MeshNameMap.find(str);
    if (iMesh != m_MeshNameMap.end())
        return m_pMeshs[iMesh->second];
    return nullptr;
}

std::shared_ptr<CTexture> CResourceManager::GetTextureFromName(std::string str)
{
    auto iTexture = m_TextureNameMap.find(str);
    if (iTexture != m_TextureNameMap.end())
        return m_pTextures[iTexture->second];
    return nullptr;
}

std::shared_ptr<CTexture> CResourceManager::ImportTexture(std::string path, GLuint samplingMethod)
{
    std::shared_ptr<CTexture> texture = std::make_shared<CTexture>();
    texture->LoadTextureFromPNG(path.c_str(), samplingMethod);

    std::string parentPath = "Textures/";
    size_t name_Start = path.find(parentPath) + parentPath.length();
    size_t name_end = path.find('.', name_Start);

    std::string textureName{ path.begin() + name_Start, path.begin() + name_end };
    texture->SetName(textureName);

    RegisterTexture(texture);

    return texture;
}

std::vector<std::string> CResourceManager::GetTextureNameList()
{
    std::vector<std::string> names;
    for (std::shared_ptr<CTexture> texture : m_pTextures) {
        names.emplace_back(texture->GetName());
    }

    return names;
}

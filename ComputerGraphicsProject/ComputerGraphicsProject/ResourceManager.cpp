#include "ResourceManager.h"
#include <string_view>
#include "Renderer.h"

using namespace std::literals;

CResourceManager::CResourceManager()
{
    m_MeshViewCamera.SetPosision(glm::vec3(1.0f));
    m_MeshViewCamera.GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
    m_MeshViewCamera.m_mat4x4View = glm::lookAt(m_MeshViewCamera.GetPosition(), glm::vec3(0), glm::vec3(0, 1, 0));
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

UINT CResourceManager::GetMaterialIndex(std::string str)
{
    auto iMat = m_MaterialNameMap.find(str);
    if (iMat != m_MaterialNameMap.end())
        return iMat->second;
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

bool CResourceManager::SwapTexture(UINT t1_index, UINT t2_index)
{
    if(t1_index >= m_pTextures.size() || t2_index >= m_pTextures.size())
        return false;

    std::shared_ptr<CTexture> temp = m_pTextures[t1_index];
    m_pTextures[t1_index] = m_pTextures[t2_index];
    m_pTextures[t2_index] = temp;

    m_TextureNameMap[m_pTextures[t2_index]->GetName()] = t2_index;
    m_TextureNameMap[m_pTextures[t1_index]->GetName()] = t1_index;

    return true;
}

bool CResourceManager::SwapMaterial(UINT t1_index, UINT t2_index)
{
    if (t1_index >= m_pMaterials.size() || t2_index >= m_pMaterials.size())
        return false;

    std::shared_ptr<CMaterial> temp = m_pMaterials[t1_index];
    m_pMaterials[t1_index] = m_pMaterials[t2_index];
    m_pMaterials[t2_index] = temp;

    m_MaterialNameMap[m_pMaterials[t2_index]->GetName()] = t2_index;
    m_MaterialNameMap[m_pMaterials[t1_index]->GetName()] = t1_index;

    return true;
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

std::shared_ptr<CTexture> CResourceManager::ImportTexture(std::string path, std::string textureName, GLuint samplingMethod)
{
    std::shared_ptr<CTexture> texture = std::make_shared<CTexture>();
    texture->LoadTextureFromPNG(path.c_str(), samplingMethod);

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

std::vector<std::string> CResourceManager::GetMaterialNameList()
{
    std::vector<std::string> names;
    for (std::shared_ptr<CMaterial> mat : m_pMaterials) {
        names.emplace_back(mat->GetName());
    }

    return names;
}

void CResourceManager::BakeViewTextures()
{
    GLuint s_Program = g_Renderer->BaseColorRenderShader;

    if (m_pMeshViewerTextures.size() < m_pMeshs.size()) {
        while (m_pMeshViewerTextures.size() < m_pMeshs.size())
        {
            std::shared_ptr<CViewerTexture> pTexture = std::make_shared<CViewerTexture>();
            m_pMeshViewerTextures.emplace_back(pTexture);
        }
    }

    for (int i = 0; i < m_pMeshViewerTextures.size(); ++i) {
        m_pMeshViewerTextures[i]->Bake(s_Program, m_pMeshs[i].get(), &m_MeshViewCamera);
    }
    m_bBakedMeshViewTextures = true;
}

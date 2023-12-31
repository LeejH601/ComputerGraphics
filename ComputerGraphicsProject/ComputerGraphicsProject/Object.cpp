#include "Object.h"
#include "Mesh.h"
#include "ResourceManager.h"

CObject* CObject::FindFrameByName(CObject* object, std::string& name)
{
	if (object == nullptr)
		return nullptr;

	if (object->GetName() == name) {
		return object;
	}

	CObject* obj = CObject::FindFrameByName(object->GetSibling(), name);

	if (obj)
		return obj;

	return CObject::FindFrameByName(object->GetChild(), name);
}

void CObject::SetScale(glm::vec3 scale)
{
	m_vec3Scale = scale;
	ReganerateTransform();
}

void CObject::RotationQuat(float radian, glm::vec3 axis)
{
	m_vec4Rotation = glm::rotate(m_vec4Rotation, radian, axis);
}

void CObject::ReganerateTransform()
{
	m_mat4x4Transform = glm::identity<glm::mat4x4>();

	m_mat4x4Transform[0][0] = m_vec3Scale.x;
	m_mat4x4Transform[1][1] = m_vec3Scale.y;
	m_mat4x4Transform[2][2] = m_vec3Scale.z;

	m_mat4x4Transform = glm::mat4_cast(m_vec4Rotation) * m_mat4x4Transform;

	m_mat4x4Transform[3][0] = m_vec3Position.x;
	m_mat4x4Transform[3][1] = m_vec3Position.y;
	m_mat4x4Transform[3][2] = m_vec3Position.z;
}

void CObject::UpdateTransform(glm::mat4x4* parent)
{
	//if(parent == nullptr)
	ReganerateTransform();
	m_mat4x4Wolrd = (parent) ? *parent * m_mat4x4Transform : m_mat4x4Transform;


	if (m_pSibling) m_pSibling->UpdateTransform(parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_mat4x4Wolrd);
}

void CObject::LoadFrameHierarchyFromFile(CObject* pParent, FILE* pInFile, int* pnSkinnedMeshes)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;
	char pstrFrameName[256];
	std::string strBuffer;

	int nFrame = 0, nTextures = 0;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			nFrame = ::ReadIntegerFromFile(pInFile);
			nTextures = ::ReadIntegerFromFile(pInFile);

			int length = ::ReadStringFromFile(pInFile, pstrFrameName); //
			m_strObjectName = pstrFrameName;
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			glm::vec3 xmf3Position, xmf3Rotation, xmf3Scale;
			glm::vec4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
			m_vec3Position = xmf3Position;
			m_vec3Scale = xmf3Scale;
			m_vec4Rotation = glm::quat(xmf4Rotation.w, xmf4Rotation.x, xmf4Rotation.y, xmf4Rotation.z);
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			nReads = (UINT)::fread(&m_mat4x4Transform, sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			std::shared_ptr<CMesh> pMesh = CMesh::LoadMeshFromFile(pInFile);

			SetMesh(pMesh);
		}
		else if (!strcmp(pstrToken, "<SkinningInfo>:"))
		{
			//if (pnSkinnedMeshes) (*pnSkinnedMeshes)++;

			//std::shared_ptr<CSkinnedMesh> pSkinnedMesh = std::make_shared<CSkinnedMesh>(pd3dDevice, pd3dCommandList);
			//pSkinnedMesh->LoadSkinInfoFromFile(pd3dDevice, pd3dCommandList, pInFile);

			//::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			//if (!strcmp(pstrToken, "<Mesh>:")) pSkinnedMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);

			//SetMesh(pSkinnedMesh);
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			LoadMaterialsFromFile(pParent, pInFile);
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					std::shared_ptr<CObject> pChild = std::make_shared<CObject>();
					pChild->LoadFrameHierarchyFromFile(this, pInFile);
					if (pChild) SetChild(pChild);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, "(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void CObject::SetChild(std::shared_ptr<CObject> pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild)
	{
		pChild->m_pParent = this;
	}
}

void CObject::SetPosition(glm::vec3 position)
{
	m_vec3Position = position;
}


void CObject::LoadMaterialsFromFile(CObject* pParent, FILE* pInFile)
{
	char pstrToken[64] = { '\0' };

	int nMaterial = 0;
	BYTE nStrLength = 0;

	UINT nReads = (UINT)::fread(&m_nMaterials, sizeof(int), 1, pInFile);

	m_ppMaterials.resize(m_nMaterials);
	for (int i = 0; i < m_nMaterials; i++) m_ppMaterials[i] = NULL;

	std::shared_ptr<CMaterial> pMaterial = NULL;
	std::shared_ptr<CTexture> pTexture = NULL;

	char pstrTextureName[64] = { '\0' };
	static int TextureLoadCnt = 3;
	float buffer;

	auto LoadTexture = [&]() {
		pTexture = std::make_shared<CTexture>();
		if (strcmp(pstrTextureName, "null"))
		{
			std::shared_ptr<CTexture> loadedTexture = CResourceManager::GetInst()->GetTextureFromName(pstrTextureName);
			if (loadedTexture == nullptr) {
				pTexture->LoadTextureFromPNG(std::string("Textures/") + std::string(pstrTextureName) + std::string(".png"), GL_LINEAR);
				pTexture->SetName(pstrTextureName);
				CResourceManager::GetInst()->RegisterTexture(pTexture);
			}
			else {
				pTexture = loadedTexture;
			}
		}
		};

	for (; ; )
	{
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0';

		if (!strcmp(pstrToken, "<Material>:"))
		{

			/*if (TextureLoadCnt < 3) {
				for(int i =0;i< 3 - TextureLoadCnt;++i)
					CModelShader::GetInst()->DescriptorHandleMoveNext();
			}*/
			//TextureLoadCnt = 0;
			nReads = (UINT)::fread(&nMaterial, sizeof(int), 1, pInFile);

			pMaterial = std::make_shared<CMaterial>();

			

			
			SetMaterial(nMaterial, pMaterial);

			//UINT nMeshType = GetMeshType();
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			glm::vec4 baseColor;
			nReads = (UINT)::fread(&(baseColor), sizeof(float), 4, pInFile);
			pMaterial->BaseColor = glm::vec3(baseColor.x, baseColor.y, baseColor.z);
			
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			glm::vec4 EmissiveColor;
			nReads = (UINT)::fread(&(EmissiveColor), sizeof(float), 4, pInFile);
			pMaterial->EmissiveColor = glm::vec3(EmissiveColor.x, EmissiveColor.y, EmissiveColor.z);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			glm::vec4 SpecularColor;
			nReads = (UINT)::fread(&(SpecularColor), sizeof(float), 4, pInFile);
			pMaterial->SpecularColor = glm::vec3(SpecularColor.x, SpecularColor.y, SpecularColor.z);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&(buffer), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&(buffer), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&(buffer), sizeof(float), 1, pInFile);
			pMaterial->MetallicColor = buffer;
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&(buffer), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&(buffer), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			//TextureLoadCnt++;
			BYTE nStrLength = 64;
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
			pstrTextureName[nStrLength] = '\0';

			LoadTexture();
			pMaterial->SetBaseTexture(pTexture);

			if (pMaterial->GetName() == "") {
				pMaterial->SetName(pTexture->GetName());
				CResourceManager::GetInst()->RegisterMaterial(pMaterial);
			}
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			BYTE nStrLength = 64;
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
			pstrTextureName[nStrLength] = '\0';

			LoadTexture();
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			BYTE nStrLength = 64;
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
			pstrTextureName[nStrLength] = '\0';

			LoadTexture();
			pMaterial->SetNormalTexture(pTexture);
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			BYTE nStrLength = 64;
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
			pstrTextureName[nStrLength] = '\0';

			LoadTexture();
			pMaterial->SetMetallicTexture(pTexture);
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			BYTE nStrLength = 64;
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
			pstrTextureName[nStrLength] = '\0';

			LoadTexture();
		}
		/*else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			if (pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pParent, pInFile, CModelShader::GetInst(), 5)) pMaterial->SetMaterialType(MATERIAL_DETAIL_ALBEDO_MAP);
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			if (pTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pParent, pInFile, CModelShader::GetInst(), 6)) pMaterial->SetMaterialType(MATERIAL_DETAIL_NORMAL_MAP);
		}*/
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			if (pMaterial->GetName() == "") {
				pMaterial->SetName(std::to_string(CResourceManager::GetInst()->GetMaterialList().size()));
				CResourceManager::GetInst()->RegisterMaterial(pMaterial);
			}
			break;
		}
	}
}

void CObject::LoadGeometryAndAnimationFromFile(const char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	std::unique_ptr<CLoadedModelInfo> pLoadedModel = std::make_unique<CLoadedModelInfo>();

	pLoadedModel->pFilePath = pstrFileName;
	pLoadedModel->m_pModelRootObject = std::make_shared<CObject>();
	pLoadedModel->m_pModelRootObject->m_strObjectName = pstrFileName;

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				this->LoadFrameHierarchyFromFile(NULL, pInFile);
				::ReadStringFromFile(pInFile, pstrToken); //"</Hierarchy>"
			}
			/*else if (!strcmp(pstrToken, "<Animation>:"))
			{
				LoadAnimationFromFile(pInFile, pLoadedModel.get());
				pLoadedModel->PrepareSkinning();
			}
			else if (!strcmp(pstrToken, "</Animation>:"))
			{
				break;
			}*/
		}
		else
		{
			break;
		}
	}
	std::cout << std::endl;
	//vModels.push_back(std::move(pLoadedModel));
	//return (vModels.back()).get();
}

CObject::CObject()
{
	m_mat4x4Transform = glm::identity<glm::mat4x4>();
	m_mat4x4Wolrd = glm::identity<glm::mat4x4>();
	m_vec3Position = { 0,0,0 };

	m_vec4Rotation = glm::quat(1, 0, 0, 0);
	m_vec3Scale = glm::vec3(1);
}

//CObject::CObject(const CObject& other)
//{
//	m_vec3Position = other.m_vec3Position;
//	m_mat4x4Transform = other.m_mat4x4Transform;
//	m_mat4x4Wolrd = other.m_mat4x4Wolrd;
//
//	m_vec4Rotation = other.m_vec4Rotation;
//	m_nMaterials = other.m_nMaterials;
//	m_ppMaterials = other.m_ppMaterials;
//	m_vec3Scale = other.m_vec3Scale;
//}

//CObject::CObject(CObject&& other) noexcept
//{
//	m_vec3Position = other.m_vec3Position;
//	m_mat4x4Transform = other.m_mat4x4Transform;
//	m_mat4x4Wolrd = other.m_mat4x4Wolrd;
//
//	m_vec4Rotation = other.m_vec4Rotation;
//	m_nMaterials = other.m_nMaterials;
//	m_ppMaterials = other.m_ppMaterials;
//	m_vec3Scale = other.m_vec3Scale;
//	
//	// 할당 해제
//	other.m_mat4x4Transform = glm::identity<glm::mat4x4>();
//	other.m_mat4x4Wolrd = glm::identity<glm::mat4x4>();
//
//	other.m_vec4Rotation = glm::quat(1, 0, 0, 0);
//	other.m_nMaterials = 0;
//	other.m_ppMaterials.clear();
//}
//
//CObject& CObject::operator=(const CObject& other)
//{
//	m_vec3Position = other.m_vec3Position;
//	m_mat4x4Transform = other.m_mat4x4Transform;
//	m_mat4x4Wolrd = other.m_mat4x4Wolrd;
//
//	m_vec4Rotation = other.m_vec4Rotation;
//	m_nMaterials = other.m_nMaterials;
//	m_ppMaterials = other.m_ppMaterials;
//	m_vec3Scale = other.m_vec3Scale;
//
//	return *this;
//}
//
//CObject& CObject::operator=(CObject&& other) noexcept
//{
//	m_vec3Position = other.m_vec3Position;
//	m_mat4x4Transform = other.m_mat4x4Transform;
//	m_mat4x4Wolrd = other.m_mat4x4Wolrd;
//
//	m_vec4Rotation = other.m_vec4Rotation;
//	m_nMaterials = other.m_nMaterials;
//	m_ppMaterials = other.m_ppMaterials;
//	m_vec3Scale = other.m_vec3Scale;
//
//	// 할당 해제
//	other.m_mat4x4Transform = glm::identity<glm::mat4x4>();
//	other.m_mat4x4Wolrd = glm::identity<glm::mat4x4>();
//
//	other.m_vec4Rotation = glm::quat(1, 0, 0, 0);
//	other.m_nMaterials = 0;
//	other.m_ppMaterials.clear();
//
//	return *this;
//}

void CObject::Update(float fElapsedTime)
{
}

void CObject::Render(GLuint s_Program)
{
	if (m_pMesh) {
		BindShaderVariables(s_Program);
		m_pMesh->Render();
	}

	if (m_pSibling)
		m_pSibling->Render(s_Program);
	if (m_pChild)
		m_pChild->Render(s_Program);
}

void CObject::BindShaderVariables(GLuint s_Program)
{
	if (m_ppMaterials.size() > 0) {
		for (std::shared_ptr<CMaterial>& pMt : m_ppMaterials) {
			pMt->BindShaderVariables(s_Program);
		}
	}

	GLuint worldLoc = glGetUniformLocation(s_Program, "worldTransform");

	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m_mat4x4Wolrd));

	if (m_pMesh) {
		m_pMesh->BindShaderVariables(s_Program);
	}
}


void CObject::SetMaterial(std::shared_ptr<CMaterial> pMaterial)
{
	if (pMaterial) {
		m_ppMaterials.push_back(pMaterial);
		m_nMaterials = m_ppMaterials.size();
	}
}


void CObject::SetMaterial(int nMaterial, std::shared_ptr<CMaterial> pMaterial)
{
	m_ppMaterials[nMaterial] = pMaterial;
}


CLoadedModelInfo::~CLoadedModelInfo()
{
}

IMoveContext::IMoveContext()
{
}

IMoveContext::~IMoveContext()
{
}

void IMoveContext::Rotate(glm::vec4& Quaternion, float angle, glm::vec3 axis)
{

}

void CRotateContext::Rotate(glm::vec4& Quaternion, float angle, glm::vec3 axis)
{
	glm::quat q = m_pObject->GetQauternion();
	m_pObject->SetQauternion(glm::rotate(q, glm::radians(angle), axis));
}

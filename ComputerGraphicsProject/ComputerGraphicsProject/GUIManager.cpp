#include "GUIManager.h"
#include "Global.h"
#include "ResourceManager.h"
#include "Object.h"

CGUIManager::CGUIManager()
{
	wrap_width = g_WindowSizeX / 11;
	wrap_width = std::floor(wrap_width);
}

CGUIManager::~CGUIManager()
{
}

void CGUIManager::ShowAssetInspector()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	static bool bIsOpenAsset = true;

	if (ImGui::Begin("Assets", &bIsOpenAsset)) {
		ImVec2 winSize = ImGui::GetWindowSize();
		ImVec2 winPos = ImGui::GetWindowPos();
		winSize.x = g_WindowSizeX * 2 / 3;
		winSize.y = g_WindowSizeY / 3;
		ImGui::SetWindowSize("Assets", winSize);
		winPos.x = 0;
		winPos.y = g_WindowSizeY * 2 / 3;
		ImGui::SetWindowPos("Assets", winPos);
	}
	else {
		ImGui::SetWindowPos("Assets", ImVec2(0, g_WindowSizeY));
	}

	frameSize = ImGui::GetWindowSize();
	//wrap_width = (float)frameSize.x / widthSize;
	widthSize = std::floor((float)frameSize.x / wrap_width);
	if (ImGui::BeginTabBar("assets")) {
		ShowTextureInspector();
		ShowMaterialInspector();

		if (ImGui::BeginTabItem("Meshs")) {
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	if (m_pSelectedObject)
		ShowSelectedObjectInfo(m_pSelectedObject);

	ImGui::Render();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CGUIManager::ShowTextureInspector()
{
	if (ImGui::BeginTabItem("textures")) {
		int index = 0;
		std::vector<std::shared_ptr<CTexture>>& pTexure = CResourceManager::GetInst()->GetTextureList();
		for (int i = 0; i < pTexure.size(); ++i) {
			ImGui::PushID(i);

			if (i % (widthSize - 1) != 0)
				ImGui::SameLine();

			ImTextureID textureID = &pTexure[i]->m_TextureID;
			ImVec2 resoultion = ImVec2(frameSize.x / widthSize, frameSize.x / widthSize);
			ImGui::BeginGroup();
			ImGui::Image((void*)pTexure[i]->m_TextureID, resoultion);


			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload("DND_DEMO_CELL", &i, sizeof(int));

				// Display preview (could be anything, e.g. when dragging an image we could decide to display
				// the filename and a small preview of the image, etc.)
				m_nSelectItemIndex = i;
				m_eDragingType = DRAGING_SOURCE_TYPE::SOURCE_TEXTURE;
				ImGui::Image((void*)pTexure[i]->m_TextureID, resoultion);
				ImGui::Text(pTexure[i]->GetName().c_str());

				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;

					std::shared_ptr<CTexture> temp = pTexure[payload_n];
					pTexure[payload_n] = pTexure[i];
					pTexure[i] = temp;
				}
				m_eDragingType = DRAGING_SOURCE_TYPE::SOURCE_NONE;
				ImGui::EndDragDropTarget();
			}

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + resoultion.x);
			ImGui::Text(pTexure[i]->GetName().c_str());

			ImGui::EndGroup();
			//ImGui::ImageButton(pTexure->GetName().c_str(), (void*)pTexure->m_TextureID, resoultion);

			ImGui::PopID();
		}
		ImGui::EndTabItem();
	}
}

void CGUIManager::ShowMaterialInspector()
{
	if (ImGui::BeginTabItem("Materials")) {
		int index = 0;
		std::vector<std::shared_ptr<CMaterial>>& pMaterials = CResourceManager::GetInst()->GetMaterialList();
		for (int i = 0; i < pMaterials.size(); ++i) {
			ImGui::PushID(i);

			if (i % (widthSize - 1) != 0)
				ImGui::SameLine();

			ImTextureID textureID;
			if (pMaterials[i]->m_pBaseTexture)
				textureID = &pMaterials[i]->m_pBaseTexture->m_TextureID;
			ImVec2 resoultion = ImVec2(frameSize.x / widthSize, frameSize.x / widthSize);
			ImGui::BeginGroup();
			if (pMaterials[i]->m_pBaseTexture)
				ImGui::Image((void*)pMaterials[i]->m_pBaseTexture->m_TextureID, resoultion);
			else
				ImGui::Image(0, resoultion);


			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload("DND_DEMO_CELL", &i, sizeof(int));

				// Display preview (could be anything, e.g. when dragging an image we could decide to display
				// the filename and a small preview of the image, etc.)
				m_nSelectItemIndex = i;
				m_eDragingType = DRAGING_SOURCE_TYPE::SOURCE_MATERIAL;
				if (pMaterials[i]->m_pBaseTexture)
					ImGui::Image((void*)pMaterials[i]->m_pBaseTexture->m_TextureID, resoultion);
				else
					ImGui::Image(0, resoultion);
				ImGui::Text(pMaterials[i]->GetName().c_str());

				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;

					std::shared_ptr<CMaterial> temp = pMaterials[payload_n];
					pMaterials[payload_n] = pMaterials[i];
					pMaterials[i] = temp;
				}
				m_eDragingType = DRAGING_SOURCE_TYPE::SOURCE_NONE;
				ImGui::EndDragDropTarget();
			}
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + resoultion.x);
			ImGui::Text(pMaterials[i]->GetName().c_str());

			ImGui::EndGroup();
			//ImGui::ImageButton(pTexure->GetName().c_str(), (void*)pTexure->m_TextureID, resoultion);

			ImGui::PopID();
		}
		ImGui::EndTabItem();
	}
}

void CGUIManager::ShowSelectedObjectInfo(CObject* obj)
{
	if (ImGui::Begin("object")) {
		ImVec2 winSize = ImGui::GetWindowSize();
		ImVec2 winPos = ImGui::GetWindowPos();
		winSize.x = g_WindowSizeX / 3;
		winSize.y = g_WindowSizeY;
		ImGui::SetWindowSize("object", winSize);
		winPos.x = g_WindowSizeX * 2 / 3;
		winPos.y = 0;
		ImGui::SetWindowPos("object", winPos);
	}

	std::vector<std::shared_ptr<CMaterial>>& pMaterials = obj->GetAllMaterials();

	if (ImGui::TreeNode("Materials")) {
		std::vector<std::string> matNames = CResourceManager::GetInst()->GetMaterialNameList();
		std::vector<const char*> matitems;
		for (std::string& str : matNames)
			matitems.emplace_back(str.c_str());

		for (int i = 0; i < pMaterials.size(); ++i) {
			int matIndex = -1;
			std::shared_ptr<CMaterial> pMat = pMaterials[i];
			if (pMat->m_pBaseTexture)
				matIndex = CResourceManager::GetInst()->GetMaterialIndex(pMat->GetName());
			std::string matTag = "Material-";
			matTag += std::to_string(i);
			if (ImGui::Combo(matTag.c_str(), &matIndex, matitems.data(), matitems.size())) {
				std::shared_ptr<CMaterial> newMat = CResourceManager::GetInst()->GetMaterialFromIndex(matIndex);
				obj->SetMaterial(i, newMat);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (m_eDragingType == DRAGING_SOURCE_TYPE::SOURCE_MATERIAL) {
					std::shared_ptr<CMaterial> newMat = CResourceManager::GetInst()->GetMaterialFromIndex(m_nSelectItemIndex);
					obj->SetMaterial(i, newMat);
				}
				ImGui::EndDragDropTarget();
			}

			//ImGui::Text("BaseColor"); ImGui::SameLine();
			std::string nameTag = std::to_string(i);
			ImGui::ColorEdit3((std::string("BaseColor##") + nameTag).c_str(), (float*)&pMat->BaseColor);
			ImGui::ColorEdit3((std::string("NormalColor##") + nameTag).c_str(), (float*)&pMat->NormalColor);
			ImGui::DragFloat((std::string("Metallic##") + nameTag).c_str(), (float*)&pMat->MetallicColor);
			ImGui::DragFloat((std::string("Roughness##") + nameTag).c_str(), (float*)&pMat->RoughnessColor);

			ImGui::InputFloat2((std::string("TexOffset##") + nameTag).c_str(), (float*)&pMat->UVOffset);
			ImGui::InputFloat2((std::string("TexSacle##") + nameTag).c_str(), (float*)&pMat->UVOffset.z);

			int baseTextureIndex = 0;
			if (pMat->m_pBaseTexture)
				baseTextureIndex = CResourceManager::GetInst()->GetTextureIndex(pMat->m_pBaseTexture->GetName()) + 1;
			int normalTextureIndex = 0;
			if (pMat->m_pNormalTexture)
				normalTextureIndex = CResourceManager::GetInst()->GetTextureIndex(pMat->m_pNormalTexture->GetName()) + 1;
			int metailicTextureIndex = 0;
			if (pMat->m_pMetallicTexture)
				metailicTextureIndex = CResourceManager::GetInst()->GetTextureIndex(pMat->m_pMetallicTexture->GetName()) + 1;
			int roughnessTextureIndex = 0;
			if (pMat->m_pRoughnessTexture)
				roughnessTextureIndex = CResourceManager::GetInst()->GetTextureIndex(pMat->m_pRoughnessTexture->GetName()) + 1;


			std::vector<std::string> textureNames = CResourceManager::GetInst()->GetTextureNameList();
			std::vector<const char*> items;
			items.emplace_back("None");
			for (std::string& str : textureNames)
				items.emplace_back(str.c_str());

			if (ImGui::Combo("BaseTexture", &baseTextureIndex, items.data(), items.size())) {
				std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(baseTextureIndex - 1);
				pMat->SetBaseTexture(newTexture);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (m_eDragingType == DRAGING_SOURCE_TYPE::SOURCE_TEXTURE) {
					std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(m_nSelectItemIndex);
					pMat->SetBaseTexture(newTexture);
				}
				ImGui::EndDragDropTarget();
			}
			GLuint baseTexID = (pMat->m_pBaseTexture) ? pMat->m_pBaseTexture->m_TextureID : -1;
			ImGui::Image((void*)baseTexID, ImVec2(20, 20));

			if (ImGui::Combo("NormalTexture", &normalTextureIndex, items.data(), items.size())) {
				std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(normalTextureIndex - 1);
				pMat->SetNormalTexture(newTexture);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (m_eDragingType == DRAGING_SOURCE_TYPE::SOURCE_TEXTURE) {
					std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(m_nSelectItemIndex);
					pMat->SetNormalTexture(newTexture);
				}
				ImGui::EndDragDropTarget();
			}
			GLuint normalTexID = (pMat->m_pNormalTexture) ? pMat->m_pNormalTexture->m_TextureID : -1;
			ImGui::Image((void*)normalTexID, ImVec2(20, 20));

			if (ImGui::Combo("MetaillicTexture", &metailicTextureIndex, items.data(), items.size())) {
				std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(metailicTextureIndex - 1);
				pMat->SetMetallicTexture(newTexture);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (m_eDragingType == DRAGING_SOURCE_TYPE::SOURCE_TEXTURE) {
					std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(m_nSelectItemIndex);
					pMat->SetMetallicTexture(newTexture);
				}
				ImGui::EndDragDropTarget();
			}
			GLuint metalTexID = (pMat->m_pMetallicTexture) ? pMat->m_pMetallicTexture->m_TextureID : -1;
			ImGui::Image((void*)metalTexID, ImVec2(20, 20));

			if (ImGui::Combo("RoughnessTexture", &roughnessTextureIndex, items.data(), items.size())) {
				std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(roughnessTextureIndex - 1);
				pMat->SetRoughnessTexture(newTexture);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (m_eDragingType == DRAGING_SOURCE_TYPE::SOURCE_TEXTURE) {
					std::shared_ptr<CTexture> newTexture = CResourceManager::GetInst()->GetTextureFromIndex(m_nSelectItemIndex);
					pMat->SetRoughnessTexture(newTexture);
				}
				ImGui::EndDragDropTarget();
			}
			GLuint roughTexID = (pMat->m_pRoughnessTexture) ? pMat->m_pRoughnessTexture->m_TextureID : -1;
			ImGui::Image((void*)roughTexID, ImVec2(20, 20));


		}
		ImGui::TreePop();
	}

	ImGui::End();
}

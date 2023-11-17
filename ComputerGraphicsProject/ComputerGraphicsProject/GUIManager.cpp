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

	if(m_pSelectedObject)
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

			if (i % (widthSize-1) != 0)
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
	if(ImGui::Begin("object")) {
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
		for (int i = 0; i < pMaterials.size(); ++i) {
			std::shared_ptr<CMaterial> pMat = pMaterials[i];
			//ImGui::Text("BaseColor"); ImGui::SameLine();
			std::string nameTag = std::to_string(i);
			ImGui::ColorEdit3((std::string("BaseColor##") + nameTag).c_str(), (float*)&pMat->BaseColor);
			ImGui::ColorEdit3((std::string("NormalColor##") + nameTag).c_str(), (float*)&pMat->NormalColor);
			ImGui::DragFloat((std::string("Metallic##") + nameTag).c_str(), (float*)&pMat->MetallicColor);
			ImGui::DragFloat((std::string("Roughness##") + nameTag).c_str(), (float*)&pMat->RoughnessColor);

			if (pMat->m_pBaseTexture) {
				UINT baseTextureIndex = CResourceManager::GetInst()->GetTextureIndex(pMat->m_pBaseTexture->GetName());
				std::vector<std::string> textureNames = CResourceManager::GetInst()->GetTextureNameList();
				//ImGui::Combo("BaseTexture", &baseTextureIndex, textureNames.data());
				ImGui::BeginCombo("BaseTexture", (const char*)&baseTextureIndex);
				ImGui::Image((void*)&pMat->m_pBaseTexture->m_TextureID, ImVec2(10, 10));
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

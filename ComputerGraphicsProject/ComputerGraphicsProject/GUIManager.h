#pragma once
#include "stdafx.h"

enum class DRAGING_SOURCE_TYPE
{
	SOURCE_NONE = -1,
	SOURCE_TEXTURE,
	SOURCE_MATERIAL,
	SOURCE_MESH,
	TYPE_END,
};

class CObject;
class CScene;
class CGUIManager
{
	ImVec2 frameSize;
	float wrap_width;
	int widthSize = 11;
	CObject* m_pSelectedObject = nullptr;

	DRAGING_SOURCE_TYPE m_eDragingType = DRAGING_SOURCE_TYPE::SOURCE_NONE;
	int m_nSelectItemIndex = -1;

public:
	DECLARE_SINGLE(CGUIManager);
	CGUIManager();
	virtual ~CGUIManager();

	void ShowAssetInspector(CScene* pScene);
	void ShowTextureInspector();
	void ShowMeshInspector();
	void ShowMaterialInspector();
	void ShowSceneInspector(CScene* pScene);
	void ShowSelectedObjectInfo(CObject* obj);
	void ShowObjectHierarchyTree(CObject* obj);

	void SetSelectedObject(CObject* obj) {
		m_pSelectedObject = obj;
	}
};


#pragma once
#include "stdafx.h"

class CObject;
class CGUIManager
{
	ImVec2 frameSize;
	float wrap_width;
	int widthSize = 11;
	CObject* m_pSelectedObject = nullptr;
public:
	DECLARE_SINGLE(CGUIManager);
	CGUIManager();
	virtual ~CGUIManager();

	void ShowAssetInspector();
	void ShowTextureInspector();
	void ShowMaterialInspector();
	void ShowSelectedObjectInfo(CObject* obj);

	void SetSelectedObject(CObject* obj) {
		m_pSelectedObject = obj;
	}
};


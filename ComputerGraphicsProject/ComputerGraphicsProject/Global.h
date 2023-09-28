#pragma once
#include "stdafx.h"



class IObject {
public:
	virtual glm::quat GetQauternion();
	virtual void SetQauternion(glm::quat rotation);
};


#define DW_FRONT 0x01
#define DW_BACK  0x02
#define DW_RIGHT 0x04
#define DW_LEFT	 0x08
#define DW_UP	 0x10
#define DW_DOWN  0x20
#define DW_SHIFT 0x40

enum class MOUSE_STATE {
	MOUSE_CILCK_LEFT,
	MOUSE_CILCK_RIGHT,
	MOUSE_CILCK_NONE,
};

extern int g_WindowSizeX ;
extern int g_WindowSizeY ;
#pragma once
#include "stdafx.h"


class IObject {
public:
	virtual glm::quat GetQauternion();
	virtual void SetQauternion(glm::quat rotation);
};
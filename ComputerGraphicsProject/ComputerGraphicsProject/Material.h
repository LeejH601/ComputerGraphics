#pragma once
#include "stdafx.h"

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();


	// 머터리얼 값들
	glm::vec4 BaseColor;
	glm::vec4 MetallicColor;
	glm::vec4 SpecularColor;
	glm::vec4 RoughnessColor;
	glm::vec4 EmissiveColor;
	glm::float32 Refraction;
	glm::float32 Opacity;

	// 텍스쳐 필요

};


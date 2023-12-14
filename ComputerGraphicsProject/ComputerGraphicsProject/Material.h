#pragma once
#include "stdafx.h"
#include "Texture.h"

#define MATERIAL_BASE_MAP			0x01
#define MATERIAL_ROUGHNESS_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_SPECULAR_MAP		0x20


class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

	virtual void BindShaderVariables(GLuint s_Program);

	std::string m_strMaterialName;

	UINT TextureMask;

	// 머터리얼 값들
	glm::vec3 BaseColor;
	glm::float32 MetallicColor;
	glm::vec3 NormalColor;
	glm::vec3 SpecularColor;
	glm::float32 RoughnessColor;
	glm::float32 FresnelColor;
	glm::vec3 EmissiveColor;
	glm::float32 EmissiveValue;
	glm::float32 Refraction;
	glm::float32 Opacity;
	glm::vec4 UVOffset;

	// 텍스쳐 필요
	std::shared_ptr<CTexture> m_pBaseTexture;
	std::shared_ptr<CTexture> m_pNormalTexture;
	std::shared_ptr<CTexture> m_pMetallicTexture;
	std::shared_ptr<CTexture> m_pRoughnessTexture;

	void SetUVOffset(float t1, float t2, float s1, float s2);
	void SetUVOffset(glm::vec4 offset);
	void SetBaseTexture(std::shared_ptr<CTexture>& pTexture);
	void SetNormalTexture(std::shared_ptr<CTexture>& pTexture);
	void SetMetallicTexture(std::shared_ptr<CTexture>& pTexture);
	void SetRoughnessTexture(std::shared_ptr<CTexture>& pTexture);
	void SetName(std::string str) { m_strMaterialName = str; };
	std::string GetName() { return m_strMaterialName; };
};


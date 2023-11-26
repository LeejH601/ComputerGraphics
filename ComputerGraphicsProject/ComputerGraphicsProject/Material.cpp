#include "Material.h"

CMaterial::CMaterial()
{
	TextureMask = 0;
	BaseColor = glm::vec3(1, 0.86, 0.57);
	SpecularColor = glm::vec3(1, 1, 1);
	MetallicColor = 1.0f;
	RoughnessColor = 0.1f;
	FresnelColor = 0.04f;
	EmissiveColor = glm::vec3(0, 0, 0);
	UVOffset = glm::vec4(0, 0, 1, 1);
}

CMaterial::~CMaterial()
{
}

void CMaterial::BindShaderVariables(GLuint s_Program)
{

	GLuint TextureMaskLoc = glGetUniformLocation(s_Program, "gTextureMask");
	glUniform1i(TextureMaskLoc, TextureMask);

	GLuint BaseLoc = glGetUniformLocation(s_Program, "gBaseColor");
	glUniform3fv(BaseLoc, 1, glm::value_ptr(BaseColor));

	GLuint SpecularLoc = glGetUniformLocation(s_Program, "gSpecularColor");
	glUniform3fv(SpecularLoc, 1, glm::value_ptr(SpecularColor));

	GLuint MetallicLoc = glGetUniformLocation(s_Program, "gMetallicColor");
	glUniform1f(MetallicLoc, MetallicColor);

	GLuint RoughnessLoc = glGetUniformLocation(s_Program, "gRoughnessColor");
	glUniform1f(RoughnessLoc, RoughnessColor);

	GLuint EmissiveLoc = glGetUniformLocation(s_Program, "gEimissiveColor");
	glUniform3fv(EmissiveLoc, 1, glm::value_ptr(EmissiveColor));

	GLuint FresnelLoc = glGetUniformLocation(s_Program, "gFresnel");
	glUniform1f(FresnelLoc, FresnelColor);

	GLuint UVOffsetLoc = glGetUniformLocation(s_Program, "gUVOffset");
	glUniform4fv(UVOffsetLoc, 1, glm::value_ptr(UVOffset));


	if (TextureMask & MATERIAL_BASE_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_BaseTexture");
		glUniform1i(samplerULoc, 0);

		m_pBaseTexture->BindShaderVariables(s_Program, GL_TEXTURE0);
	}
	if (TextureMask & MATERIAL_NORMAL_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_NormalTexture");
		glUniform1i(samplerULoc, 1);

		m_pNormalTexture->BindShaderVariables(s_Program, GL_TEXTURE1);
	}
	if (TextureMask & MATERIAL_ROUGHNESS_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_RoughnessTexture");
		glUniform1i(samplerULoc, 2);

		m_pRoughnessTexture->BindShaderVariables(s_Program, GL_TEXTURE2);
	}
	if (TextureMask & MATERIAL_METALLIC_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_MetallicTexture");
		glUniform1i(samplerULoc, 3);

		m_pMetallicTexture->BindShaderVariables(s_Program, GL_TEXTURE3);
	}
	if (TextureMask & MATERIAL_EMISSION_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_EmissionTexture");
		glUniform1i(samplerULoc, 4);

		m_pEmissionTexture->BindShaderVariables(s_Program, GL_TEXTURE4);
	}
	if (TextureMask & MATERIAL_AO_MAP) {
		GLuint samplerULoc = glGetUniformLocation(s_Program, "u_AOTexture");
		glUniform1i(samplerULoc, 9);

		m_pAOTexture->BindShaderVariables(s_Program, GL_TEXTURE9);
	}
}

void CMaterial::SetUVOffset(float t1, float t2, float s1, float s2)
{
	UVOffset = glm::vec4(t1, t2, s1, s2);
}

void CMaterial::SetUVOffset(glm::vec4 offset)
{
	UVOffset = offset;
}

void CMaterial::SetBaseTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pBaseTexture = pTexture;

	if (pTexture != nullptr)
		TextureMask |= MATERIAL_BASE_MAP;
	else
		TextureMask -= MATERIAL_BASE_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

void CMaterial::SetNormalTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pNormalTexture = pTexture;
	if (pTexture != nullptr)
		TextureMask |= MATERIAL_NORMAL_MAP;
	else
		TextureMask -= MATERIAL_NORMAL_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

void CMaterial::SetMetallicTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pMetallicTexture = pTexture;
	if (pTexture != nullptr)
		TextureMask |= MATERIAL_METALLIC_MAP;
	else
		TextureMask -= MATERIAL_METALLIC_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

void CMaterial::SetRoughnessTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pRoughnessTexture = pTexture;
	if (pTexture != nullptr)
		TextureMask |= MATERIAL_ROUGHNESS_MAP;
	else
		TextureMask -= MATERIAL_ROUGHNESS_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

void CMaterial::SetAOTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pAOTexture = pTexture;
	if (pTexture != nullptr)
		TextureMask |= MATERIAL_AO_MAP;
	else
		TextureMask -= MATERIAL_AO_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

void CMaterial::SetEmissionTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pEmissionTexture = pTexture;
	if (pTexture != nullptr)
		TextureMask |= MATERIAL_EMISSION_MAP;
	else
		TextureMask -= MATERIAL_EMISSION_MAP;
	TextureMask = std::max((UINT)0, TextureMask);
}

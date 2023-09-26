#include "Material.h"

CMaterial::CMaterial()
{
	TextureMask = 0;
	BaseColor = glm::vec3(1, 0, 0);
	SpecularColor = glm::vec3(1, 1, 1);
	MetallicColor = 0.04f;
	RoughnessColor = 0.1f;
	FresnelColor = 0.04f;
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

	GLuint FresnelLoc = glGetUniformLocation(s_Program, "gFresnel");
	glUniform1f(FresnelLoc, FresnelColor);


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
	if (TextureMask & MATERIAL_SPECULAR_MAP) {

	}
	if (TextureMask & MATERIAL_METALLIC_MAP) {

	}
	if (TextureMask & MATERIAL_EMISSION_MAP) {

	}
		
}

void CMaterial::SetBaseTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pBaseTexture = pTexture;
	TextureMask |= MATERIAL_BASE_MAP;
}

void CMaterial::SetNormalTexture(std::shared_ptr<CTexture>& pTexture)
{
	m_pNormalTexture = pTexture;
	TextureMask |= MATERIAL_NORMAL_MAP;
}

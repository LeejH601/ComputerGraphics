#include "Material.h"

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::BindShaderVariables(GLuint s_Program)
{

	GLuint BaseLoc = glGetUniformLocation(s_Program, "gBaseColor");
	glUniform3fv(BaseLoc, 1, glm::value_ptr(BaseColor));

	GLuint SpecularLoc = glGetUniformLocation(s_Program, "gSpecularColor");
	glUniform3fv(SpecularLoc, 1, glm::value_ptr(SpecularColor));

	GLuint MetallicLoc = glGetUniformLocation(s_Program, "gMetallicColor");
	glUniform1f(MetallicLoc, MetallicColor);

	GLuint RoughnessLoc = glGetUniformLocation(s_Program, "gRoughnessColor");
	glUniform1f(RoughnessLoc, MetallicColor);

	if (TextureMask & MATERIAL_BASE_MAP) {

	}
	if (TextureMask & MATERIAL_NORMAL_MAP) {

	}
	if (TextureMask & MATERIAL_SPECULAR_MAP) {

	}
	if (TextureMask & MATERIAL_METALLIC_MAP) {

	}
	if (TextureMask & MATERIAL_EMISSION_MAP) {

	}
	GLuint worldLoc = glGetUniformLocation(s_Program, "worldTransform");

	//glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(m_mat4x4Wolrd));
}

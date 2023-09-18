#include "Light.h"

void CLight::CreateShaderVariables()
{

}

void CLight::BindShaderVariables(GLuint s_Program)
{
	GLuint LightColorLoc = glGetUniformLocation(s_Program, "gMainLight.vec3LightColor");
	GLuint LightIntensLoc = glGetUniformLocation(s_Program, "gMainLight.vec3LightColor");
	GLuint LightDirLoc = glGetUniformLocation(s_Program, "gMainLight.vec3Direction");

	glUniform3fv(LightColorLoc, 1, glm::value_ptr(m_vec3Direction));
	glUniform1f(LightIntensLoc, m_fIntensity);
	glUniform3fv(LightDirLoc, 1, glm::value_ptr(m_vec3Direction));
}

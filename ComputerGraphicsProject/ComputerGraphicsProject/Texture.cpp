#include "Texture.h"
#include "LoadPng.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION

#include "Dependencies/stb-master/stb_image.h"

CTexture::CTexture()
{

}

CTexture::~CTexture()
{
}

void CTexture::BindShaderVariables(GLuint s_Program, GLenum TextureSlot)
{
	glActiveTexture(TextureSlot);
	glBindTexture(m_TextureType, m_TextureID);
}

void CTexture::LoadTextureFromPNG(std::string filePath, GLuint samplingMethod)
{
	std::vector<unsigned char> image;
	unsigned width, height;
	
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
		assert(0);
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	m_nWidth = width;
	m_nHeight = height;
	m_TextureType = GL_TEXTURE_2D;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);

}

void CTexture::LoadTextureHDR(std::string filePath, GLuint samplingMethod)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(filePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		m_TextureType = GL_TEXTURE_2D;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}
}

CViewerTexture::CViewerTexture()
{
}

CViewerTexture::~CViewerTexture()
{
}


void CViewerTexture::Bake(GLuint s_Program, CMesh* pMesh, CCamera* pCamera)
{
	if (m_bBaked)
		return;

	static GLuint s_viewerFBO = -1;
	static GLuint s_viewerRBO = -1;

	GLenum err = glGetError();
	err = glGetError();

	if(s_viewerFBO == -1)
		glGenFramebuffers(1, &s_viewerFBO);
	if (s_viewerRBO == -1)
		glGenRenderbuffers(1, &s_viewerRBO);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	m_nWidth = 512;
	m_nHeight = 512;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	

	glUseProgram(s_Program);

	glBindFramebuffer(GL_FRAMEBUFFER, s_viewerFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, s_viewerRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_nWidth, m_nHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	

	//glViewport(0, 0, m_nWidth, m_nHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	pCamera->m_vec4ViewPort = glm::vec4(0, 0, m_nWidth, m_nHeight);
	pCamera->BindShaderVariables(s_Program, false);

	

	CMaterial MeshMaterial;
	MeshMaterial.BaseColor = glm::vec3(1, 1, 1);

	MeshMaterial.BindShaderVariables(s_Program);

	GLuint worldLoc = glGetUniformLocation(s_Program, "worldTransform");

	glm::mat4 mat4x4Wolrd = glm::identity<glm::mat4>();
	glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(mat4x4Wolrd));

	pMesh->BindShaderVariables(s_Program);
	err = glGetError();
	pMesh->Render();

	

	m_bBaked = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

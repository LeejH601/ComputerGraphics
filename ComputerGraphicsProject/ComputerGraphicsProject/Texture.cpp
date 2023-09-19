#include "Texture.h"
#include "LoadPng.h"

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

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);
}

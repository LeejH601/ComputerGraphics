#include "stdafx.h"
#include "Renderer.h"
#include "assert.h"
#include "Object.h"
#include "Mesh.h"

CRenderer::CRenderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


CRenderer::~CRenderer()
{
}

void CRenderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	//m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs",
		//"./Shaders/SolidRect.fs");

	TestShader = CompileShaders((char*)"./Shaders/PBR_vs.glsl", (char*)"./Shaders/PBR_ps.glsl");
	PointInstanceShader = CompileShaders((char*)"./Shaders/PBRInstance_vs.glsl", (char*)"./Shaders/Color_ps.glsl");
	CubeShader = CompileShaders((char*)"./Shaders/Cube_vs.glsl", (char*)"./Shaders/Cube_ps.glsl");
	MakeCubeMapShader = CompileShaders((char*)"./Shaders/Cube_vs.glsl", (char*)"./Shaders/Cube_ps.glsl");
	SkyBoxShader = CompileShaders((char*)"./Shaders/environmentMake_vs.glsl", (char*)"./Shaders/environmentMake_ps.glsl");
	irradianceShader = CompileShaders((char*)"./Shaders/irradianceMake_vs.glsl", (char*)"./Shaders/irradianceMake_ps.glsl");
	preFilteringCubeMapShader = CompileShaders((char*)"./Shaders/FilteringedEnvironmentMake_vs.glsl", (char*)"./Shaders/FilteringedEnvironmentMake_ps.glsl");
	preComputingBRDFShader = CompileShaders((char*)"./Shaders/RenderNDCQuad_vs.glsl", (char*)"./Shaders/preComputeBRDF_ps.glsl");
	LineShader = CompileShaders((char*)"./Shaders/Line_vs.glsl", (char*)"./Shaders/Line_ps.glsl");

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	


	m_Initialized = true;

}



bool CRenderer::IsInitialized()
{
	return m_Initialized;
}


void CRenderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

int CRenderer::MakeVertexShader(const char* vertexPath, int n)
{
	unsigned int ID;

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::ifstream vShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);

		std::stringstream vShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();

	// 2. compile shaders
	//unsigned int vertex, fragment;

	// vertex shader
	ID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ID, 1, &vShaderCode, NULL);
	glCompileShader(ID);
	checkCompileErrors(ID, "VERTEX");
	return ID;
}

int CRenderer::MakeFragmentShader(const char* fragmentPath, int count)
{
	unsigned int ID;

	// 1. retrieve the vertex/fragment source code from filePath

	std::string fragmentCode;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		fShaderFile.close();

		// convert stream into string
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	//unsigned int vertex, fragment;

	// fragment Shader
	ID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ID, 1, &fShaderCode, NULL);
	glCompileShader(ID);
	checkCompileErrors(ID, "FRAGMENT");

	return ID;
}

void CRenderer::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

bool CRenderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint CRenderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	//AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	glAttachShader(ShaderProgram, MakeVertexShader(filenameVS, 0));

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	//AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	glAttachShader(ShaderProgram, MakeFragmentShader(filenameFS, 0));

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done." << std::endl;

	return ShaderProgram;
}



void CRenderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}


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
	CubeShader = CompileShaders((char*)"./Shaders/Cube_vs.glsl", (char*)"./Shaders/Cube_ps.glsl");
	MakeCubeMapShader = CompileShaders((char*)"./Shaders/Cube_vs.glsl", (char*)"./Shaders/Cube_ps.glsl");
	SkyBoxShader = CompileShaders((char*)"./Shaders/environmentMake_vs.glsl", (char*)"./Shaders/environmentMake_ps.glsl");
	irradianceShader = CompileShaders((char*)"./Shaders/irradianceMake_vs.glsl", (char*)"./Shaders/irradianceMake_ps.glsl");

	// HDRI 로부터 큐브 환경 맵 생성
	GLuint cubeFBO, cubeRBO;
	GLuint cubeMapWidth = 1024, cubeMapHeight = 1024;
	glGenFramebuffers(1, &cubeFBO);
	glGenRenderbuffers(1, &cubeRBO);

	glBindBuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, cubeRBO, cubeMapWidth, cubeMapHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, cubeRBO);

	GLuint& cubeMapID = m_tCubeMapTexture.m_TextureID;
	glGenTextures(1, &cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
	m_tCubeMapTexture.m_TextureType = GL_TEXTURE_CUBE_MAP;
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeMapWidth, cubeMapHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 tempCaptureProjection = glm::perspective(glm::radians(90.f), 1.0f, 0.1f, 10.0f);
	glm::mat4 tempCaptureViews[6] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	glUseProgram(MakeCubeMapShader); // 셰이더 설정

	GLuint samplerULoc = glGetUniformLocation(MakeCubeMapShader, "u_BaseTexture");
	glUniform1i(samplerULoc, 0);

	//m_tCubeMapTexture.BindShaderVariables(MakeCubeMapShader, GL_TEXTURE_CUBE_MAP);

	GLuint projectionLocation;
	projectionLocation = glGetUniformLocation(MakeCubeMapShader, "projectionTransform");	// ProjMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &tempCaptureProjection[0][0]);

	std::shared_ptr<CTexture> pTexture = std::make_shared<CTexture>();
	//pTexture->LoadTextureFromPNG("./Textures/rgb.png", GL_NEAREST);
	pTexture->LoadTextureHDR("./Textures/poly_haven_studio_4k.hdr", GL_LINEAR);
	pTexture->BindShaderVariables(MakeCubeMapShader, GL_TEXTURE0);

	std::shared_ptr<CMesh> pCubeMesh = std::make_shared<CMesh>();
	pCubeMesh = CMesh::CreateCubeMesh(1.f, 1.f, 1.f);
	pCubeMesh->CreateShaderVariables();

	glViewport(0, 0, cubeMapWidth, cubeMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	for (GLuint i = 0; i < 6; ++i) {
		GLuint viewLocation;
		viewLocation = glGetUniformLocation(MakeCubeMapShader, "viewTransform");	// ViewMatrix
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &tempCaptureViews[i][0][0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pCubeMesh->BindShaderVariables(MakeCubeMapShader);
		pCubeMesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 생성된 큐브 환경 맵에서 irradiance 큐브 맵 생성
	m_tIrradianceTexture = std::make_shared<CTexture>();
	GLuint& IrradianceID = m_tIrradianceTexture->m_TextureID;
	GLuint IrradianceMapWidth = 64, IrradianceMapHeight = 64;
	glGenTextures(1, &IrradianceID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceID);
	m_tIrradianceTexture->m_TextureType = GL_TEXTURE_CUBE_MAP;
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IrradianceMapWidth, IrradianceMapHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindBuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, cubeRBO, IrradianceMapWidth, IrradianceMapHeight);

	glUseProgram(irradianceShader);

	samplerULoc = glGetUniformLocation(irradianceShader, "u_BaseTexture");
	glUniform1i(samplerULoc, 0);

	//m_tCubeMapTexture.BindShaderVariables(MakeCubeMapShader, GL_TEXTURE_CUBE_MAP);

	projectionLocation;
	projectionLocation = glGetUniformLocation(irradianceShader, "projectionTransform");	// ProjMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &tempCaptureProjection[0][0]);

	m_tCubeMapTexture.BindShaderVariables(irradianceShader, GL_TEXTURE0);

	glViewport(0, 0, IrradianceMapWidth, IrradianceMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	for (GLuint i = 0; i < 6; ++i) {
		GLuint viewLocation;
		viewLocation = glGetUniformLocation(irradianceShader, "viewTransform");	// ViewMatrix
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &tempCaptureViews[i][0][0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pCubeMesh->BindShaderVariables(irradianceShader);
		pCubeMesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_Initialized = true;

}



bool CRenderer::IsInitialized()
{
	return m_Initialized;
}


void CRenderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	//AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	glAttachShader(ShaderProgram, MakeVertexShader(filenameVS, 0));

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	//AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	glAttachShader(ShaderProgram, MakeFragmentShader(filenameFS, 0));

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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


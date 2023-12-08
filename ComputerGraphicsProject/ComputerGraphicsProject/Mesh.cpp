#include "Mesh.h"
#include "ResourceManager.h"
#include "Animation.h"

std::shared_ptr<CMesh> CMesh::CreateCubeMesh(float fWidth, float fHeight, float fDepth)
{
	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->SetName("myCube");
	mesh->m_nVertices = 36;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	static unsigned int cubeShapeindex[] = {
		// À°¸éÃ¼
		3, 2, 6, // +x
		3, 6, 7,
		4, 5, 1, // -x
		4, 1, 0,
		4, 0, 3, // +y
		4, 3, 7,
		1, 5, 6, // -y
		1, 6, 2,
		0, 1, 2, // +z
		0, 2, 3,
		7, 6, 5, // -z
		7, 5, 4
	};


	static GLfloat cubeNormal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
	};

	static unsigned int cubeNormalindex[] = {
		// À°¸éÃ¼
		2, 2, 2,
		2, 2, 2,
		3, 3, 3,
		3, 3, 3,
		4, 4, 4,
		4, 4, 4,
		5, 5, 5,
		5, 5, 5,
		0, 0, 0,
		0, 0, 0,
		1, 1, 1,
		1, 1, 1
	};


	std::vector<glm::vec3> Vertices; Vertices.resize(8);

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;


	Vertices[0] = glm::vec3(-fx, fy, fz);
	Vertices[1] = glm::vec3(-fx, -fy, fz);
	Vertices[2] = glm::vec3(fx, -fy, fz);
	Vertices[3] = glm::vec3(fx, fy, fz);
	Vertices[4] = glm::vec3(-fx, fy, -fz);
	Vertices[5] = glm::vec3(-fx, -fy, -fz);
	Vertices[6] = glm::vec3(fx, -fy, -fz);
	Vertices[7] = glm::vec3(fx, fy, -fz);

	for (int i = 0; i < mesh->m_nVertices; ++i) {
		mesh->m_pVertices[i].position = Vertices[cubeShapeindex[i]];
		mesh->m_pVertices[i].normal = glm::vec3(cubeNormal[cubeNormalindex[i]][0], cubeNormal[cubeNormalindex[i]][1], cubeNormal[cubeNormalindex[i]][2]);
	}

	return mesh;
}

std::shared_ptr<CMesh> CMesh::CreateCubeMeshForIndex(float fWidth, float fHeight, float fDepth)
{
	static unsigned int cubeShapeindex[] = {
		// À°¸éÃ¼
		0, 1, 2,
		0, 2, 3,
		7, 6, 5,
		7, 5, 4,
		3, 2, 6,
		3, 6, 7,
		4, 5, 1,
		4, 1, 0,
		4, 0, 3,
		4, 3, 7,
		1, 5, 6,
		1, 6, 2
	};

	static unsigned int cubeShapeindex2[] = {
		// À°¸éÃ¼
		0, 2, 1,
		0, 3, 2,
		7, 5, 6,
		7, 4, 5,
		3, 6, 2,
		3, 7, 6,
		4, 1, 5,
		4, 0, 1,
		4, 3, 0,
		4, 7, 3,
		1, 6, 5,
		1, 1, 6
	};

	//static GLuint indices[] = {
	//	// Front face
	//	0, 1, 2, 2, 3, 0,

	//	// Back face
	//	4, 5, 6, 6, 7, 4,

	//	// Top face
	//	0, 3, 7, 7, 4, 0,

	//	// Bottom face
	//	1, 2, 6, 6, 5, 1,

	//	// Left face
	//	0, 1, 5, 5, 4, 0,

	//	// Right face
	//	2, 3, 7, 7, 6, 2
	//};

	static GLuint indices[] = {
	0, 1, 2, 3, 7, 6, 5, 4, 0, 1
	};

	static GLfloat cubeNormal[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0}
	};

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->SetName("myCubeIndexed");
	mesh->m_nVertices = 8;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	mesh->m_pVertices[0].position = glm::vec3(-fx, fy, fz);
	mesh->m_pVertices[1].position = glm::vec3(-fx, -fy, fz);
	mesh->m_pVertices[2].position = glm::vec3(fx, -fy, fz);
	mesh->m_pVertices[3].position = glm::vec3(fx, fy, fz);
	mesh->m_pVertices[4].position = glm::vec3(-fx, fy, -fz);
	mesh->m_pVertices[5].position = glm::vec3(-fx, -fy, -fz);
	mesh->m_pVertices[6].position = glm::vec3(fx, -fy, -fz);
	mesh->m_pVertices[7].position = glm::vec3(fx, fy, -fz);


	mesh->m_pVertices[0].normal = glm::normalize(mesh->m_pVertices[0].position);
	mesh->m_pVertices[1].normal = glm::normalize(mesh->m_pVertices[1].position);
	mesh->m_pVertices[2].normal = glm::normalize(mesh->m_pVertices[2].position);
	mesh->m_pVertices[3].normal = glm::normalize(mesh->m_pVertices[3].position);
	mesh->m_pVertices[4].normal = glm::normalize(mesh->m_pVertices[4].position);
	mesh->m_pVertices[5].normal = glm::normalize(mesh->m_pVertices[5].position);
	mesh->m_pVertices[6].normal = glm::normalize(mesh->m_pVertices[6].position);
	mesh->m_pVertices[7].normal = glm::normalize(mesh->m_pVertices[7].position);

	mesh->m_nSubMeshes = 1;
	mesh->m_pnSubSetIndices.resize(mesh->m_nSubMeshes);
	mesh->m_pnSubSetIndices[0] = 36;
	mesh->m_ppnSubSetIndices.resize(mesh->m_nSubMeshes);
	mesh->m_ppnSubSetIndices[0].resize(mesh->m_pnSubSetIndices[0]);

	for (int i = 0; i < mesh->m_pnSubSetIndices[0]; ++i) {
		mesh->m_ppnSubSetIndices[0][i] = cubeShapeindex[i];
	}


	return mesh;
}

std::shared_ptr<CMesh> CMesh::CreateSphereMesh(int nStacks, int nSlices)
{
	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->SetName("mySphere");
	//m_nStride = sizeof(CIlluminatedVertex);

	int k = 0;

	static float m_PI = 3.141592f;
	static float fRadius = 0.5f; // ÀÓ½Ã

	float fDeltaPhi = float(m_PI / nStacks);
	float fDeltaTheta = float((2.0f * m_PI) / nSlices);

	UINT nVertices = 2 + (nSlices * (nStacks - 1));
	mesh->m_nVertices = nVertices;

	mesh->m_pVertices.resize(nVertices);

	mesh->m_pVertices[k++].position = glm::vec3(0.f, +fRadius, 0.f);

	float theta_i, phi_j;
	for (UINT j = 1; j < nStacks; j++)
	{
		phi_j = fDeltaPhi * j;
		for (UINT i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			mesh->m_pVertices[k].position = glm::vec3(fRadius * sinf(phi_j) * cosf(theta_i), fRadius * cosf(phi_j), fRadius * sinf(phi_j) * sinf(theta_i));
			mesh->m_pVertices[k++].normal = glm::normalize(mesh->m_pVertices[k].position - glm::vec3(0, 0, 0));
		}
	}
	mesh->m_pVertices[k].position = glm::vec3(0.0f, -fRadius, 0.0f);

	mesh->m_nSubMeshes = 1;
	UINT nIndices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);
	mesh->m_pnSubSetIndices.resize(1);

	mesh->m_pnSubSetIndices[0] = nIndices;
	mesh->m_ppnSubSetIndices.resize(mesh->m_nSubMeshes);
	mesh->m_ppnSubSetIndices[0].resize(nIndices);

	k = 0;
	for (UINT i = 0; i < nSlices; i++)
	{
		mesh->m_ppnSubSetIndices[0][k++] = 0;
		mesh->m_ppnSubSetIndices[0][k++] = 1 + ((i + 1) % nSlices);
		mesh->m_ppnSubSetIndices[0][k++] = 1 + i;
	}
	for (UINT j = 0; j < nStacks - 2; j++)
	{
		for (UINT i = 0; i < nSlices; i++)
		{
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (i + (j * nSlices));
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (i + ((j + 1) * nSlices));
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (i + ((j + 1) * nSlices));
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			mesh->m_ppnSubSetIndices[0][k++] = 1 + (((i + 1) % nSlices) + ((j + 1) * nSlices));
		}
	}
	for (UINT i = 0; i < nSlices; i++)
	{
		mesh->m_ppnSubSetIndices[0][k++] = (nVertices - 1);
		mesh->m_ppnSubSetIndices[0][k++] = ((nVertices - 1) - nSlices) + i;
		mesh->m_ppnSubSetIndices[0][k++] = ((nVertices - 1) - nSlices) + ((i + 1) % nSlices);
	}

	/*CalculateVertexNormals(pxmf3Normals, pxmf3Positions, m_nVertices, pnIndices, m_nIndices);

	CIlluminatedVertex* pVertices = new CIlluminatedVertex[m_nVertices];
	for (UINT i = 0; i < m_nVertices; i++) pVertices[i] = CIlluminatedVertex(pxmf3Positions[i], pxmf3Normals[i]);
	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_d3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_d3dPositionBufferView.StrideInBytes = m_nStride;
	m_d3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;

	if (pxmf3Positions) delete[] pxmf3Positions;
	if (pxmf3Normals) delete[] pxmf3Normals;
	if (pVertices) delete[] pVertices;

	m_nSubMeshes = 1;
	m_pnSubSetIndices = new int[m_nSubMeshes];
	m_pnSubSetIndices[0] = (int)m_nIndices;
	m_ppd3dSubSetIndexBuffers = new ID3D12Resource * [m_nSubMeshes];
	m_ppd3dSubSetIndexBuffers[0] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_ppd3dSubSetIndexBuffers[0]);

	m_pd3dSubSetIndexBufferViews = new D3D12_INDEX_BUFFER_VIEW[m_nSubMeshes];
	m_pd3dSubSetIndexBufferViews[0].BufferLocation = m_ppd3dSubSetIndexBuffers[0]->GetGPUVirtualAddress();
	m_pd3dSubSetIndexBufferViews[0].Format = DXGI_FORMAT_R32_UINT;
	m_pd3dSubSetIndexBufferViews[0].SizeInBytes = sizeof(UINT) * m_nIndices;*/

	return mesh;
}

std::shared_ptr<CMesh> CMesh::CreateNDCMesh()
{
	static glm::vec3 vertexPos[] = {
		// x, y, z
		{-1.0f,  1.0f, 0.0f},
		{-1.0f, -1.0f, 0.0f},
		{ 1.0f,  1.0f, 0.0f},
		{ 1.0f,  1.0f, 0.0f},
		{ -1.0f,-1.0f, 0.0f},
		{ 1.0f, -1.0f, 0.0f}
	};

	static glm::vec2 texcoord[] = {
		// x, y, z
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f,  1.0f},
		{1.0f,  1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f}
	};

	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->SetName("myNDCPlane");
	mesh->m_nVertices = 6;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	for (int i = 0; i < mesh->m_nVertices; ++i) {
		mesh->m_pVertices[i].position = vertexPos[i];
		mesh->m_pVertices[i].texcoord0 = texcoord[i];
	}

	return mesh;
}

std::shared_ptr<CMesh> CMesh::CreatePointMesh()
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	pMesh->SetName("myPoint");
	pMesh->m_PrimitiveTopology = GL_POINTS;

	pMesh->m_nVertices = 1;
	pMesh->m_pVertices.resize(pMesh->m_nVertices);
	pMesh->m_pVertices[0].position = (glm::vec3(0, 0, 0));
	pMesh->m_pVertices[0].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[0].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[0].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[0].texcoord0 = glm::vec2(0, 0);

	return pMesh;
}

std::shared_ptr<CMesh> CMesh::CreateLineMesh()
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	pMesh->SetName("myLine");
	pMesh->m_PrimitiveTopology = GL_LINES;

	pMesh->m_nVertices = 2;
	pMesh->m_pVertices.resize(pMesh->m_nVertices);
	pMesh->m_pVertices[0].position = (glm::vec3(0, 0, 0.0f));
	pMesh->m_pVertices[0].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[0].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[0].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[0].texcoord0 = glm::vec2(0, 0);

	pMesh->m_pVertices[1].position = (glm::vec3(0, 0, 1.0));
	pMesh->m_pVertices[1].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[1].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[1].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[1].texcoord0 = glm::vec2(1, 0);

	return pMesh;
}

std::shared_ptr<CMesh> CMesh::CreateTriangleMesh()
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	pMesh->SetName("myTriangle");
	pMesh->m_PrimitiveTopology = GL_TRIANGLES;

	pMesh->m_nVertices = 3;
	pMesh->m_pVertices.resize(pMesh->m_nVertices);
	pMesh->m_pVertices[0].position = (glm::vec3(0, 0, -1.0f));
	pMesh->m_pVertices[0].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[0].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[0].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[0].texcoord0 = glm::vec2(0.5, 0.5);

	pMesh->m_pVertices[1].position = (glm::vec3(-0.5, 0, 1.0));
	pMesh->m_pVertices[1].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[1].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[1].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[1].texcoord0 = glm::vec2(0, 0);

	pMesh->m_pVertices[2].position = (glm::vec3(0.5, 0, 1.0));
	pMesh->m_pVertices[2].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[2].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[2].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[2].texcoord0 = glm::vec2(1, 0);

	return pMesh;
}

std::shared_ptr<CMesh> CMesh::CreateRectMesh()
{
	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	pMesh->SetName("myRect");
	pMesh->m_PrimitiveTopology = GL_TRIANGLES;

	pMesh->m_nVertices = 6;
	pMesh->m_pVertices.resize(pMesh->m_nVertices);

	pMesh->m_pVertices[0].position = (glm::vec3(-0.5f, 0, -0.5f));
	pMesh->m_pVertices[0].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[0].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[0].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[0].texcoord0 = glm::vec2(0, 1);

	pMesh->m_pVertices[1].position = (glm::vec3(-0.5, 0, 0.5));
	pMesh->m_pVertices[1].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[1].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[1].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[1].texcoord0 = glm::vec2(0, 0);

	pMesh->m_pVertices[2].position = (glm::vec3(0.5, 0, -0.5));
	pMesh->m_pVertices[2].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[2].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[2].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[2].texcoord0 = glm::vec2(1, 1);


	pMesh->m_pVertices[3].position = (glm::vec3(0.5, 0, -0.5));
	pMesh->m_pVertices[3].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[3].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[3].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[3].texcoord0 = glm::vec2(1, 1);

	pMesh->m_pVertices[4].position = (glm::vec3(-0.5, 0, 0.5));
	pMesh->m_pVertices[4].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[4].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[4].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[4].texcoord0 = glm::vec2(0, 0);

	pMesh->m_pVertices[5].position = (glm::vec3(0.5, 0, 0.5));
	pMesh->m_pVertices[5].normal = (glm::vec3(0, 1, 0));
	pMesh->m_pVertices[5].tangent = (glm::vec3(1, 0, 0));
	pMesh->m_pVertices[5].bitangent = (glm::vec3(0, 0, 1));
	pMesh->m_pVertices[5].texcoord0 = glm::vec2(1, 0);




	return pMesh;
}

std::shared_ptr<CMesh> CMesh::LoadMeshFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	std::shared_ptr<CMesh> pMesh = std::make_shared<CMesh>();

	UINT nReads = (UINT)::fread(&pMesh->m_nVertices, sizeof(int), 1, pInFile);

	char pstrMeshName[256];
	::ReadStringFromFile(pInFile, pstrMeshName);
	pMesh->m_strMeshName = pstrMeshName;

	bool isLoadMeshed = false;
	std::shared_ptr<CMesh> loadedMesh = CResourceManager::GetInst()->GetMeshFromName(pMesh->m_strMeshName);
	if (loadedMesh != nullptr) {
		isLoadMeshed = true;
	}

	/*std::wstring wMeshName;
	size_t tmp = 0;
	wMeshName.resize(strlen(pstrMeshName) + 1);
	mbstowcs_s(&tmp, wMeshName.data(), (size_t)wMeshName.size(), pstrMeshName, (size_t)wMeshName.size());
	OutputDebugString(wMeshName.c_str());
	OutputDebugString(L"\n");*/

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			glm::vec3 m_vec3AABBCenter;
			glm::vec3 m_vec3AABBExtents;
			nReads = (UINT)::fread(&m_vec3AABBCenter, sizeof(glm::vec3), 1, pInFile);
			nReads = (UINT)::fread(&m_vec3AABBExtents, sizeof(glm::vec3), 1, pInFile);

			if (isLoadMeshed == false) {
				pMesh->m_vec3AABBCenter = m_vec3AABBCenter;
				pMesh->m_vec3AABBExtents = m_vec3AABBExtents;
			}

			/*std::wstring wBounds = L"m_xmf3AABBCenter.x: ";
			wBounds = wBounds.append(std::to_wstring(m_xmf3AABBCenter.x));
			wBounds = wBounds.append(L" m_xmf3AABBCenter.y: ");
			wBounds = wBounds.append(std::to_wstring(m_xmf3AABBCenter.y));
			wBounds = wBounds.append(L" m_xmf3AABBCenter.z: ");
			wBounds = wBounds.append(std::to_wstring(m_xmf3AABBCenter.z));
			OutputDebugString(wBounds.c_str());
			OutputDebugString(L"\n");

			std::wstring wExtents = L"m_xmf3AABBExtents.x: ";
			wExtents = wExtents.append(std::to_wstring(m_xmf3AABBExtents.x));
			wExtents = wExtents.append(L" m_xmf3AABBExtents.y: ");
			wExtents = wExtents.append(std::to_wstring(m_xmf3AABBExtents.y));
			wExtents = wExtents.append(L" m_xmf3AABBExtents.z: ");
			wExtents = wExtents.append(std::to_wstring(m_xmf3AABBExtents.z));
			OutputDebugString(wExtents.c_str());
			OutputDebugString(L"\n");*/
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			if (nPositions > 0)
			{
				//m_nType |= VERTEXT_POSITION;
				std::vector<glm::vec3> pxmf3Positions;
				pxmf3Positions.resize(nPositions);
				nReads = (UINT)::fread(pxmf3Positions.data(), sizeof(glm::vec3), nPositions, pInFile);

				if (isLoadMeshed == false) {
					if (pMesh->m_pVertices.size() == 0)
						pMesh->m_pVertices.resize(nPositions);

					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].position = pxmf3Positions[i];
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				//m_nType |= VERTEXT_COLOR;
				std::vector<glm::vec4> m_pxmf4Colors;
				m_pxmf4Colors.resize(nColors);
				nReads = (UINT)::fread(m_pxmf4Colors.data(), sizeof(glm::vec4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				//m_nType |= VERTEXT_TEXTURE_COORD0;
				std::vector<glm::vec2> m_pxmf2TextureCoords0;
				m_pxmf2TextureCoords0.resize(nTextureCoords);
				nReads = (UINT)::fread(m_pxmf2TextureCoords0.data(), sizeof(glm::vec2), nTextureCoords, pInFile);

				if (isLoadMeshed == false) {
					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].texcoord0 = m_pxmf2TextureCoords0[i];
					}
				}

			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				//m_nType |= VERTEXT_TEXTURE_COORD1;
				std::vector<glm::vec2> m_pxmf2TextureCoords1;
				m_pxmf2TextureCoords1.resize(nTextureCoords);
				nReads = (UINT)::fread(m_pxmf2TextureCoords1.data(), sizeof(glm::vec2), nTextureCoords, pInFile);

				if (isLoadMeshed == false) {
					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].texcoord1 = m_pxmf2TextureCoords1[i];
					}
				}

			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			if (nNormals > 0)
			{
				//m_nType |= VERTEXT_NORMAL;
				std::vector<glm::vec3> m_pxmf3Normals;
				m_pxmf3Normals.resize(nNormals);
				nReads = (UINT)::fread(m_pxmf3Normals.data(), sizeof(glm::vec3), nNormals, pInFile);

				if (isLoadMeshed == false) {
					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].normal = m_pxmf3Normals[i];
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			if (nTangents > 0)
			{
				//m_nType |= VERTEXT_TANGENT;
				std::vector<glm::vec3> m_pxmf3Tangents;
				m_pxmf3Tangents.resize(nTangents);
				nReads = (UINT)::fread(m_pxmf3Tangents.data(), sizeof(glm::vec3), nTangents, pInFile);

				if (isLoadMeshed == false) {
					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].tangent = m_pxmf3Tangents[i];
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				std::vector<glm::vec3> m_pxmf3BiTangents;
				m_pxmf3BiTangents.resize(nBiTangents);
				nReads = (UINT)::fread(m_pxmf3BiTangents.data(), sizeof(glm::vec3), nBiTangents, pInFile);

				if (isLoadMeshed == false) {
					for (int i = 0; i < nPositions; ++i) {
						pMesh->m_pVertices[i].bitangent = m_pxmf3BiTangents[i];
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			UINT nSubMeshes;
			nReads = (UINT)::fread(&(nSubMeshes), sizeof(int), 1, pInFile);
			if (nSubMeshes > 0)
			{
				if (isLoadMeshed == false) {
					pMesh->m_nSubMeshes = nSubMeshes;
					pMesh->m_pnSubSetIndices.resize(pMesh->m_nSubMeshes);
					pMesh->m_ppnSubSetIndices.resize(pMesh->m_nSubMeshes);


					for (int i = 0; i < pMesh->m_nSubMeshes; i++)
					{
						::ReadStringFromFile(pInFile, pstrToken);
						if (!strcmp(pstrToken, "<SubMesh>:"))
						{
							int nIndex = 0;
							nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
							nReads = (UINT)::fread(&(pMesh->m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
							if (pMesh->m_pnSubSetIndices[i] > 0)
							{
								// ¼­ºê ¸Þ½¬µéÀÇ ÀÎµ¦½º µé
								pMesh->m_ppnSubSetIndices[i].resize(pMesh->m_pnSubSetIndices[i]);
								nReads = (UINT)::fread(pMesh->m_ppnSubSetIndices[i].data(), sizeof(UINT), pMesh->m_pnSubSetIndices[i], pInFile);


							}
						}
					}
				}
				else {
					std::vector<UINT> pnSubSetIndices;
					std::vector<std::vector<UINT>> ppnSubSetIndices;
					pnSubSetIndices.resize(nSubMeshes);
					ppnSubSetIndices.resize(nSubMeshes);

					for (int i = 0; i < nSubMeshes; i++)
					{
						::ReadStringFromFile(pInFile, pstrToken);
						int nIndex = 0;

						if (!strcmp(pstrToken, "<SubMesh>:"))
						{

							nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
							nReads = (UINT)::fread(&(pnSubSetIndices[i]), sizeof(int), 1, pInFile);
							if (pnSubSetIndices[i] > 0)
							{
								ppnSubSetIndices[i].resize(pnSubSetIndices[i]);
								nReads = (UINT)::fread(ppnSubSetIndices[i].data(), sizeof(UINT), pnSubSetIndices[i], pInFile);
							}
						}
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
	if (isLoadMeshed == false) {
		pMesh->CreateShaderVariables();
		CResourceManager::GetInst()->RegisterMesh(pMesh);
	}
	else {
		pMesh = loadedMesh;
	}
	return pMesh;
}

void CMesh::SetSubmesh(std::vector<UINT>& subsetIndices)
{
	if (m_nSubMeshes == 0) {
		m_nSubMeshes = 1;
		m_pnSubSetIndices.resize(m_nSubMeshes);
		m_pnSubSetIndices[0] = subsetIndices.size();
		m_ppnSubSetIndices.resize(m_nSubMeshes);
		m_ppnSubSetIndices[0] = subsetIndices;
	}
}

void CMesh::CreateShaderVariables()
{
	if (m_bInit)
		return;
	//glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	//glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CMesh::Vertex) * m_pVertices.size(), m_pVertices.data(), GL_STATIC_DRAW);

	m_IBOs.resize(m_nSubMeshes);
	for (int i = 0; i < m_nSubMeshes; ++i) {
		glGenBuffers(1, &m_IBOs[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
		long long test = m_pnSubSetIndices[i] * sizeof(UINT);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pnSubSetIndices[i] * sizeof(UINT), m_ppnSubSetIndices[i].data(), GL_STATIC_DRAW);
	}

	m_bInit = true;
}

void CMesh::BindShaderVariables(GLuint s_Program)
{
	
	GLuint posLoc = glGetAttribLocation(s_Program, "v_Position");
	glEnableVertexAttribArray(posLoc);
	GLuint normalLoc = glGetAttribLocation(s_Program, "v_Normal");
	glEnableVertexAttribArray(normalLoc);
	GLuint tex0Loc = glGetAttribLocation(s_Program, "v_texcoord0");
	glEnableVertexAttribArray(tex0Loc);
	GLuint TangentLoc = glGetAttribLocation(s_Program, "v_Tangent");
	glEnableVertexAttribArray(TangentLoc);
	GLuint BiTangentLoc = glGetAttribLocation(s_Program, "v_Bitangent");
	glEnableVertexAttribArray(BiTangentLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), 0);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(TangentLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(BiTangentLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 9));
	glVertexAttribPointer(tex0Loc, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 12));

	GLuint meshTypeLoc = glGetUniformLocation(s_Program, "gMeshType");
	glUniform1i(meshTypeLoc, m_MeshType);
}

void CMesh::Render()
{
	if (m_nSubMeshes > 0) {
		for (int i = 0; i < m_nSubMeshes; ++i) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_ppnSubSetIndices[i].data());
			glDrawElements(m_PrimitiveTopology, m_pnSubSetIndices[i] * sizeof(UINT), GL_UNSIGNED_INT, NULL);
		}
	}
	else {
		glDrawArrays(m_PrimitiveTopology, 0, m_nVertices);
	}
}

void CMesh::RenderInstanced(int nInstance)
{
	if (m_nSubMeshes > 0) {
		for (int i = 0; i < m_nSubMeshes; ++i) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_ppnSubSetIndices[i].data());
			glDrawElementsInstanced(m_PrimitiveTopology, m_pnSubSetIndices[i] * sizeof(UINT), GL_UNSIGNED_INT, NULL, nInstance);
		}
	}
	else {
		glDrawArraysInstanced(m_PrimitiveTopology, 0, m_nVertices, nInstance);
	}
}

GLuint CSkinnedMesh::m_UBOBindPoseOffset = -1;
CSkinnedMesh::CSkinnedMesh() {
	m_MeshType = 1;
}

void CSkinnedMesh::CopyData(const CMesh& mesh)
{
	this->m_strMeshName = mesh.m_strMeshName; ;

	this->m_nVertices = mesh.m_nVertices;
	this->m_pVertices = mesh.m_pVertices;

	this->m_nIndices = mesh.m_nIndices;
	this->m_pnIndices = mesh.m_pnIndices;

	this->m_pnSubSetIndices = mesh.m_pnSubSetIndices;
	this->m_pnSubSetStartIndices = mesh.m_pnSubSetStartIndices;
	this->m_ppnSubSetIndices = mesh.m_ppnSubSetIndices;

	this->m_VAO = mesh.m_VAO;
	this->m_VBO = mesh.m_VBO;
	this->m_IBOs = mesh.m_IBOs;

	this->m_vec3AABBCenter = mesh.m_vec3AABBCenter;
	this->m_vec3AABBExtents = mesh.m_vec3AABBExtents;

	this->m_PrimitiveTopology = mesh.m_PrimitiveTopology;

	this->m_nSlot = mesh.m_nSlot;
	this->m_nSubMeshes = mesh.m_nSubMeshes;
	this->m_nStride = mesh.m_nStride;
	this->m_nOffset = mesh.m_nOffset;
	this->m_nStartIndex = mesh.m_nStartIndex;
	this->m_nBaseVertex = mesh.m_nBaseVertex;
}

void CSkinnedMesh::LoadSkinInfoFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;
	char pstrMeshName[256];
	::ReadStringFromFile(pInFile, pstrMeshName);

	bool isLoadMeshed = false;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<BonesPerVertex>:"))
		{
			m_nBonesPerVertex = ::ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Bounds>:"))
		{
			glm::vec3 m_vec3AABBCenter;
			glm::vec3 m_vec3AABBExtents;
			nReads = (UINT)::fread(&m_vec3AABBCenter, sizeof(glm::vec3), 1, pInFile);
			nReads = (UINT)::fread(&m_vec3AABBExtents, sizeof(glm::vec3), 1, pInFile);

			if (isLoadMeshed == false) {
				m_vec3AABBCenter = m_vec3AABBCenter;
				m_vec3AABBExtents = m_vec3AABBExtents;
			}
		}
		else if (!strcmp(pstrToken, "<BoneNames>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				m_ppstrSkinningBoneNames.resize(m_nSkinningBones);
				//m_ppSkinningBoneFrameCaches.resize(m_nSkinningBones);
				for (int i = 0; i < m_nSkinningBones; i++)
				{
					::ReadStringFromFile(pInFile, const_cast<char*>(m_ppstrSkinningBoneNames[i].data()));
					//m_ppSkinningBoneFrameCaches[i] = NULL;
				}
			}
		}
		else if (!strcmp(pstrToken, "<BoneOffsets>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				m_pxmf4x4BindPoseBoneOffsets.resize(m_nSkinningBones);
				nReads = (UINT)::fread(m_pxmf4x4BindPoseBoneOffsets.data(), sizeof(glm::mat4x4), m_nSkinningBones, pInFile);

			}
		}
		else if (!strcmp(pstrToken, "<BoneIndices>:"))
		{
			//m_nType |= VERTEXT_BONE_INDEX_WEIGHT;

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				std::vector<glm::ivec4> pivec4BoneIndices;
				pivec4BoneIndices.resize(m_nVertices);

				nReads = (UINT)::fread(pivec4BoneIndices.data(), sizeof(glm::ivec4), m_nVertices, pInFile);
				for (int i = 0; i < m_nVertices; ++i) {
					m_pVertices[i].m_pxmn4BoneIndices = pivec4BoneIndices[i];
				}
			}
		}
		else if (!strcmp(pstrToken, "<BoneWeights>:"))
		{
			//m_nType |= VERTEXT_BONE_INDEX_WEIGHT;

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				std::vector<glm::vec4> pvec4BoneWeights;
				pvec4BoneWeights.resize(m_nVertices);

				nReads = (UINT)::fread(pvec4BoneWeights.data(), sizeof(glm::vec4), m_nVertices, pInFile);
				for (int i = 0; i < m_nVertices; ++i) {
					m_pVertices[i].m_pxmf4BoneWeights = pvec4BoneWeights[i];
				}
			}
		}
		else if (!strcmp(pstrToken, "</SkinningInfo>"))
		{
			break;
		}
	}
	CreateShaderVariables();
}

void CSkinnedMesh::BindShaderVariables(GLuint s_Program)
{
	CMesh::BindShaderVariables(s_Program);

	GLuint indiceLoc = glGetAttribLocation(s_Program, "v_BoneIndices");
	glEnableVertexAttribArray(indiceLoc);
	GLuint weightLoc = glGetAttribLocation(s_Program, "v_BoneWeights");
	glEnableVertexAttribArray(weightLoc);

	glVertexAttribPointer(indiceLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 14));
	glVertexAttribPointer(weightLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 18));


	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBindPoseOffset);
	GLuint offset = 0u;

	//glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(UBOLightData), &UBOLightData);
	//glBindBuffer(GL_UNIFORM_BUFFER, 1);
}

void CSkinnedMesh::CreateShaderVariables()
{
	CMesh::CreateShaderVariables();

	if (m_UBOBindPoseOffset == -1) {
		glGetError();
		int size = m_pxmf4x4BindPoseBoneOffsets.size() * sizeof(glm::mat4x4);
		glGenBuffers(1, &m_UBOBindPoseOffset);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBindPoseOffset);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_UBOBindPoseOffset);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBindPoseOffset);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4x4) * m_pxmf4x4BindPoseBoneOffsets.size(), m_pxmf4x4BindPoseBoneOffsets.data());
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

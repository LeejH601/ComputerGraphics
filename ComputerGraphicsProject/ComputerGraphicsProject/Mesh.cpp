#include "Mesh.h"

std::shared_ptr<CMesh> CMesh::CreateCubeMesh(float fWidth, float fHeight, float fDepth)
{
	std::shared_ptr<CMesh> mesh = std::make_shared<CMesh>();

	mesh->m_nVertices = 36;
	mesh->m_pVertices.resize(mesh->m_nVertices);

	static unsigned int cubeShapeindex[] = {
		// À°¸éÃ¼
		0, 1, 2, // +z
		0, 2, 3,
		7, 6, 5, // -z
		7, 5, 4,
		3, 2, 6, // +x
		3, 6, 7,
		4, 5, 1, // -x
		4, 1, 0,
		4, 0, 3, // +y
		4, 3, 7,
		1, 5, 6, // -y
		1, 6, 2
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
		0, 0, 0,
		0, 0, 0,
		1, 1, 1,
		1, 1, 1,
		2, 2, 2,
		2, 2, 2,
		3, 3, 3,
		3, 3, 3,
		4, 4, 4,
		4, 4, 4,
		5, 5, 5,
		5, 5, 5
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

	//m_nStride = sizeof(CIlluminatedVertex);

	int k = 0;

	static float m_PI = 3.141592f;
	static float fRadius = 5.f; // ÀÓ½Ã

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

void CMesh::LoadMeshFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	UINT nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pInFile);

	char pstrMeshName[256];
	::ReadStringFromFile(pInFile, pstrMeshName);
	m_strMeshName = pstrMeshName;

	std::wstring wMeshName;
	size_t tmp = 0;
	wMeshName.resize(strlen(pstrMeshName) + 1);
	mbstowcs_s(&tmp, wMeshName.data(), (size_t)wMeshName.size(), pstrMeshName, (size_t)wMeshName.size());
	OutputDebugString(wMeshName.c_str());
	OutputDebugString(L"\n");

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_vec3AABBCenter, sizeof(glm::vec3), 1, pInFile);
			nReads = (UINT)::fread(&m_vec3AABBExtents, sizeof(glm::vec3), 1, pInFile);

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

				if (m_pVertices.size() == 0)
					m_pVertices.resize(nPositions);

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].position = pxmf3Positions[i];
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

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].texcoord0 = m_pxmf2TextureCoords0[i];
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

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].texcoord1 = m_pxmf2TextureCoords1[i];
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

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].normal = m_pxmf3Normals[i];
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

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].tangent = m_pxmf3Tangents[i];
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

				for (int i = 0; i < nPositions; ++i) {
					m_pVertices[i].bitangent = m_pxmf3BiTangents[i];
				}
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&(m_nSubMeshes), sizeof(int), 1, pInFile);
			if (m_nSubMeshes > 0)
			{
				m_pnSubSetIndices.resize(m_nSubMeshes);
				m_ppnSubSetIndices.resize(m_nSubMeshes);


				for (int i = 0; i < m_nSubMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (m_pnSubSetIndices[i] > 0)
						{
							// ¼­ºê ¸Þ½¬µéÀÇ ÀÎµ¦½º µé
							m_ppnSubSetIndices[i].resize(m_pnSubSetIndices[i]);
							nReads = (UINT)::fread(m_ppnSubSetIndices[i].data(), sizeof(UINT), m_pnSubSetIndices[i], pInFile);

							
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
	CreateShaderVariables();
}

void CMesh::CreateShaderVariables()
{
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
}

void CMesh::BindShaderVariables(GLuint s_Program)
{
	GLuint posLoc = glGetAttribLocation(s_Program, "v_Position");
	glEnableVertexAttribArray(posLoc);
	GLuint normalLoc = glGetAttribLocation(s_Program, "v_Normal");
	glEnableVertexAttribArray(normalLoc);
	GLuint tex0Loc = glGetAttribLocation(s_Program, "v_texcoord0");
	glEnableVertexAttribArray(tex0Loc);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), 0);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(tex0Loc, 2, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex),
		(GLvoid*)(sizeof(float) * 12));
}

void CMesh::Render()
{
	if (m_nSubMeshes > 0) {
		for (int i = 0; i < m_nSubMeshes; ++i) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOs[i]);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, m_ppnSubSetIndices[i].data());
			glDrawElements(GL_TRIANGLES, m_pnSubSetIndices[i] * sizeof(UINT), GL_UNSIGNED_INT, NULL);
		}
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
	}
}

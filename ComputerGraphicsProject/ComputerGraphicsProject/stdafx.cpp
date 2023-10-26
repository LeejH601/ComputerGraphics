#include "stdafx.h"


int ReadIntegerFromFile(FILE* pInFile)
{
	int nValue = 0;
	UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile);
	return(nValue);
}
float ReadFloatFromFile(FILE* pInFile)
{
	float fValue = 0;
	UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile);
	return(fValue);
}
BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken)
{
	BYTE nStrLength = 0;
	UINT nReads = 0;
	nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
	pstrToken[nStrLength] = '\0';

	return(nStrLength);
}

float Distance(glm::vec4 plane, glm::vec3 p)
{
	float numer = plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w;
	float denom = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	
	if (denom > 0.000000001f)
		return numer / denom;

	return 0.0f;
}

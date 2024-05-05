#include "Aakaar.h"

#include <malloc.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "detail/AakaarUtils.h"

AakaarStatus GetCubeVertices(_Out_ float** vertices)
{
	float cubeVertices[] = {
		// front face
		1.0f, 1.0f, 1.0f,		// right top
		-1.0f, 1.0f, 1.0f,		// left top
		-1.0f, -1.0f, 1.0f,		// left bottom
		1.0f, -1.0f, 1.0f,		// right bottom

		// right face
		1.0f, 1.0f, -1.0f,		// right top
		1.0f, 1.0f, 1.0f,		// left top
		1.0f, -1.0f, 1.0f,		// left bottom
		1.0f, -1.0f, -1.0f,		// right bottom

		// back face
		1.0f, 1.0f, -1.0f,		// right top
		-1.0f, 1.0f, -1.0f,		// left top
		-1.0f, -1.0f, -1.0f,	// left bottom
		1.0f, -1.0f, -1.0f,		// right bottom

		// left face
		-1.0f, 1.0f, 1.0f,		// right top
		-1.0f, 1.0f, -1.0f,		// left top
		-1.0f, -1.0f, -1.0f,	// left bottom
		-1.0f, -1.0f, 1.0f,		// right bottom

		// top face
		1.0f, 1.0f, -1.0f,		// right top
		-1.0f, 1.0f, -1.0f,		// left top
		-1.0f, 1.0f, 1.0f,		// left bottom
		1.0f, 1.0f, 1.0f,		// right bottom

		// bottom face
		1.0f, -1.0f, -1.0f,		// right top
		-1.0f, -1.0f, -1.0f,	// left top
		-1.0f, -1.0f, 1.0f,		// left bottom
		1.0f, -1.0f, 1.0f		// right bottom
	};

	if (vertices == NULL)
	{
		return AakaarStatus::AAKAAR_FAILED;
	}

	const int numberOfFaces = NUM_CUBE_FACES;
	const int numberOfVerticesPerFace = NUM_VERTICES_PER_CUBE_FACE;
	float destinationSize = CUBE_VERTICES_SIZE;

	*vertices = (float*)malloc(destinationSize);
	if (*vertices == NULL)
	{
		return AakaarStatus::AAKAAR_FAILED;
	}

	// copy the cubeVertices array into dynamically allocated array
	if (memcpy_s(*vertices, destinationSize, cubeVertices, destinationSize) != 0)
	{
		free(*vertices);
		*vertices = NULL;
		return AakaarStatus::AAKAAR_FAILED;
	}

	return AakaarStatus::AAKAAR_OK;
}


AakaarStatus GetTorusVertices(
	_Out_ VERTEX** vertices, 
	_Out_ int* numVertices, 
	_Out_ NORMAL** normals, 
	_Out_ int* numNormals, 
	_In_ double innerRadius, 
	_In_ double outerRadius, 
	_In_ int numSides, 
	_In_ int numRings)
{
	if (vertices == NULL || normals == NULL || numVertices == NULL || numNormals == NULL)
	{
		return AakaarStatus::AAKAAR_FAILED;
	}

	*vertices = NULL;
	*numVertices = 0;
	*normals = NULL;
	*numNormals = 0;

	const float ringDelta = 2.0f * M_PI / numRings;
	const float sideDelta = 2.0f * M_PI / numSides;

	float theta, theta1;
	float cosTheta, sinTheta;
	float cosTheta1, sinTheta1;

	theta = 0.0f;
	cosTheta = 1.0f;
	sinTheta = 0.0f;

	std::vector<VERTEX> tempVertices;
	std::vector<NORMAL> tempNormals;

	for (int i = numRings - 2; i >= 0; i--)
	{
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);

		generateQuadStripVertices(
			tempVertices, 
			tempNormals, 
			innerRadius, 
			outerRadius, 
			numSides, 
			sideDelta, 
			cosTheta, 
			sinTheta, 
			cosTheta1, 
			sinTheta1);

		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;

		generateQuadStripVertices(tempVertices, 
			tempNormals, 
			innerRadius, 
			outerRadius, 
			numSides, 
			sideDelta, 
			cosTheta, 
			sinTheta, 
			cosTheta1, 
			sinTheta1);
	}

	cosTheta1 = 1.0f;
	sinTheta1 = 0.0f;
	
	generateQuadStripVertices(tempVertices,
		tempNormals,
		innerRadius,
		outerRadius,
		numSides,
		sideDelta,
		cosTheta,
		sinTheta,
		cosTheta1,
		sinTheta1);


	*numVertices = tempVertices.size();
	const int verticesDestSize = (*numVertices) * sizeof(VERTEX);

	*vertices = (VERTEX*) malloc(verticesDestSize);
	if (*vertices == NULL)
	{
		return AakaarStatus::AAKAAR_FAILED;
	}

	if (memcpy_s(*vertices, verticesDestSize, tempVertices.data(), verticesDestSize) != 0)
	{
		free(*vertices);
		*vertices = NULL;
		*numVertices = 0;

		return AakaarStatus::AAKAAR_FAILED;
	}

	*numNormals = tempNormals.size();
	const int normalsDestSize = (*numNormals) * sizeof(NORMAL);
	
	*normals = (NORMAL*) malloc(normalsDestSize);
	if (*normals == NULL)
	{
		free(*vertices);
		*vertices = NULL;
		*numVertices = 0;

		return AakaarStatus::AAKAAR_FAILED;
	}

	if (memcpy_s(*normals, normalsDestSize, tempNormals.data(), normalsDestSize) != 0)
	{
		free(*vertices);
		*vertices = NULL;
		*numVertices = 0;
		
		free(*normals);
		*normals = NULL;
		*numNormals = 0;
		
		return AakaarStatus::AAKAAR_FAILED;
	}

	return AakaarStatus::AAKAAR_OK;
}
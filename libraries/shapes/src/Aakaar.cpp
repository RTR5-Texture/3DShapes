#include "Aakaar.h"

#include <malloc.h>
#include <string.h>

AakaarStatus GetCubeVertices(float** ptr)
{
	float vertices[] = {
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

	if (ptr == NULL) 
	{
		return AAKAAR_FAILED;
	}

	const int numberOfFaces = 6;
	const int numberOfVerticesPerFace = 12;
	float destinationSize = sizeof(float) * numberOfFaces * numberOfVerticesPerFace;

	*ptr = (float*)malloc(destinationSize);
	if (*ptr == NULL)
	{
		return AAKAAR_FAILED;
	}

	// copy the vertices array into dynamically allocated array
	if (memcpy_s(*ptr, destinationSize, vertices, destinationSize) != 0)
	{
		free(*ptr);
		*ptr = NULL;
		return AAKAAR_FAILED;
	}

	return AAKAAR_OK;
}
#pragma once
#include "detail/Api.h"

#include<sal.h>

#define NUM_CUBE_FACES 6
#define NUM_VERTICES_PER_CUBE_FACE 12
#define CUBE_VERTICES_SIZE \
  sizeof(float) * NUM_CUBE_FACES* NUM_VERTICES_PER_CUBE_FACE


AMC_TEXTURE_AAKAAR_API enum class AakaarStatus
{
	AAKAAR_OK,
	AAKAAR_FAILED,
	AAKAAR_NOT_INITIALIZED
};

extern "C" AMC_TEXTURE_AAKAAR_API
typedef struct _POINT_DATA
{
	float x;
	float y;
	float z;
} VERTEX, NORMAL;


extern "C" AMC_TEXTURE_AAKAAR_API 
AakaarStatus GetCubeVertices(_Out_ float** vertices);

extern "C" AMC_TEXTURE_AAKAAR_API 
AakaarStatus GetTorusVertices(
	_Out_ VERTEX** vertices, 
	_Out_ int* numVertices,
	_Out_ NORMAL** normals,
	_Out_ int* numNormals,
	_In_ const double innerRadius, 
	_In_ const double outerRadius, 
	_In_ const int numSides, 
	_In_ const int numRings);


extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCubeVertices(float**);

//extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetIntegerValue(int*);

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GenerateCylinder(float, float, int, int);

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCylinderVertices(float**);

extern "C" AMC_TEXTURE_AAKAAR_API int GetCylinderVerticesCount(void);

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCylinderNormals(float**);

extern "C" AMC_TEXTURE_AAKAAR_API int GetCylinderNormalsCount(void);

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCylinderTexcords(float**);

extern "C" AMC_TEXTURE_AAKAAR_API int GetCylinderTexcordsCount(void);

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCylinderIndices(int**);

extern "C" AMC_TEXTURE_AAKAAR_API int GetCylinderIndicesCount(void);
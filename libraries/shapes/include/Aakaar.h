#pragma once
#include <vector>

#include "detail/Api.h"

AMC_TEXTURE_AAKAAR_API enum AakaarStatus
{
	AAKAAR_OK,
	AAKAAR_FAILED
};

#define NUM_CUBE_FACES 6
#define NUM_VERTICES_PER_CUBE_FACE 12
#define CUBE_VERTICES_SIZE \
  sizeof(float) * NUM_CUBE_FACES* NUM_VERTICES_PER_CUBE_FACE


extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCubeVertices(float**);
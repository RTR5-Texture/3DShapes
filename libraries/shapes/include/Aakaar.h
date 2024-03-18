#pragma once
#include <vector>

#include "detail/Api.h"

AMC_TEXTURE_AAKAAR_API enum AakaarStatus
{
	AAKAAR_OK,
	AAKAAR_FAILED
};

extern "C" AMC_TEXTURE_AAKAAR_API AakaarStatus GetCubeVertices(float**);
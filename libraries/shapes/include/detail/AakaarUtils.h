#pragma once

#include "Aakaar.h"

#include <sal.h>
#include <vector>

void generateQuadStripVertices(
    _Out_ std::vector<VERTEX>& vertices,
    _Out_ std::vector<NORMAL>& normals,
    _In_ const float innerRadius,
    _In_ const float outerRadius,
    _In_ const int numSides,
    _In_ const float sideDelta,
    _In_ const float cosTheta,
    _In_ const float sinTheta,
    _In_ float cosTheta1,
    _In_ const float sinTheta1);
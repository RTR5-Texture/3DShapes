#include "detail/AakaarUtils.h"

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
    _In_ const float sinTheta1)
{
    float dist = 0.0f, phi = 0.0f;

    dist = innerRadius + outerRadius;

    normals.push_back(NORMAL{ cosTheta1, -sinTheta1, 0.0f });
    vertices.push_back(VERTEX{ cosTheta1 * dist, -sinTheta1 * dist, 0.0f });

    normals.push_back(NORMAL{ cosTheta, -sinTheta, 0.0f });
    vertices.push_back(VERTEX{ cosTheta * dist, -sinTheta * dist, 0.0f });

    phi = sideDelta;
    for (int j = numSides; j >= 0; j--)
    {
        float cosPhi, sinPhi;

        cosPhi = cos(phi);
        sinPhi = sin(phi);
        dist = outerRadius + innerRadius * cosPhi;

        normals.push_back(NORMAL{ cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi });
        vertices.push_back(VERTEX{ cosTheta1 * dist, -sinTheta1 * dist, innerRadius * sinPhi });

        normals.push_back(NORMAL{ cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi });
        vertices.push_back(VERTEX{ cosTheta * dist, -sinTheta * dist,  innerRadius * sinPhi });
        phi += sideDelta;
    }

    dist = innerRadius + outerRadius;

    normals.push_back(NORMAL{ cosTheta1, -sinTheta1, 0.0f });
    vertices.push_back(VERTEX{ cosTheta1 * dist, -sinTheta1 * dist, 0.0f });

    normals.push_back(NORMAL{ cosTheta, -sinTheta, 0.0f });
    vertices.push_back(VERTEX{ cosTheta * dist, -sinTheta * dist, 0.0f });
}
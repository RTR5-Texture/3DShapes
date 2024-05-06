#include "Aakaar.h"
#include <stdlib.h>

#define _USE_MATH_DEFINES 1
#include<math.h>

float *cylinder_vertices = nullptr;
float *cylinder_normals = nullptr;
float *cylinder_texcords = nullptr;
int *cylinder_indices = nullptr;

int cylinder_vertices_count = 0;
int cylinder_normals_count = 0;
int cylinder_texcords_count = 0;
int cylinder_indices_count = 0;

void Cylinder(
    float radius, float height, int slices, int stacks, /*  input */
    float **vertices, float **normals, int* nVert           /* output */
    );

void CircleTable(float **sint, float **cost, const int n, const bool halfCircle);

AakaarStatus GenerateCylinder(float radius, float height, int slices, int stacks)
{
    int i,j,idx, nVert;
    Cylinder(radius,height,slices,stacks,&cylinder_vertices,&cylinder_normals,&cylinder_vertices_count);
    return AakaarStatus::AAKAAR_OK;
}

AakaarStatus GetCylinderVertices(float** vertices)
{
    if(cylinder_vertices_count == 0)
    {
        return AakaarStatus::AAKAAR_NOT_INITIALIZED;
    }
    *vertices = cylinder_vertices;
    return AakaarStatus::AAKAAR_OK;
}

int GetCylinderVerticesCount(void)
{
    return cylinder_vertices_count;
}

AakaarStatus GetCylinderNormals(float** normals)
{
    if(cylinder_normals_count == 0)
    {
        return AakaarStatus::AAKAAR_NOT_INITIALIZED;
    }
    *normals = cylinder_normals;
    return AakaarStatus::AAKAAR_OK;
}

int GetCylinderNormalsCount(void)
{
    return cylinder_normals_count;
}

AakaarStatus GetCylinderTexcords(float** texcords)
{
    if(cylinder_texcords_count == 0)
    {
        return AakaarStatus::AAKAAR_NOT_INITIALIZED;
    }
    *texcords = cylinder_texcords;
    return AakaarStatus::AAKAAR_OK;
}

int GetCylinderTexcordsCount(void)
{
    return cylinder_texcords_count;
}

AakaarStatus GetCylinderIndices(int** indices)
{
    if(cylinder_indices_count == 0)
    {
        return AakaarStatus::AAKAAR_NOT_INITIALIZED;
    }
    *indices = cylinder_indices;
    return AakaarStatus::AAKAAR_OK;
}

int GetCylinderIndicesCount(void)
{
	return cylinder_indices_count;
}


void Cylinder(
    float radius, float height, int slices, int stacks, /*  input */
    float **vertices, float **normals, int* nVert           /* output */
    )
{
    int i,j;
    int idx = 0;    /* idx into vertex/normal buffer */

    /* Step in z as stacks are drawn. */
    float radf = (float)radius;
    float z;
    const float zStep = (float)height / ( ( stacks > 0 ) ? stacks : 1 );

    /* Pre-computed circle */
    float *sint,*cost;

    /* number of unique vertices */
    if (slices==0 || stacks<1)
    {
        /* nothing to generate */
        *nVert = 0;
        *vertices = NULL;
        *normals = NULL;
        return;
    }
    *nVert = slices*(stacks+3)+2;   /* need two extra stacks for closing off top and bottom with correct normals */

    //if ((*nVert) > 65535)
        /*
         * limit of glushort, thats 256*256 subdivisions, should be enough in practice. See note above
         */
        //fgWarning("fghGenerateCylinder: too many slices or stacks requested, indices will wrap");

    /* Pre-computed circle */
    CircleTable(&sint,&cost,-slices,false);

    /* Allocate vertex and normal buffers, bail out if memory allocation fails */
    *vertices = (float*)malloc((*nVert)*3*sizeof(float));
    *normals  = (float*)malloc((*nVert)*3*sizeof(float));
    if (!(*vertices) || !(*normals))
    {
        free(*vertices);
        free(*normals);
        *vertices = NULL;
        *normals = NULL;
        //fgError("Failed to allocate memory in fghGenerateCylinder");
        return;
    }

    z=0;
    /* top on Z-axis */
    (*vertices)[0] =  0.f;
    (*vertices)[1] =  0.f;
    (*vertices)[2] =  0.f;
    (*normals )[0] =  0.f;
    (*normals )[1] =  0.f;
    (*normals )[2] = -1.f;
    idx = 3;
    /* other on top (get normals right) */
    for (j=0; j<slices; j++, idx+=3)
    {
        (*vertices)[idx  ] = cost[j]*radf;
        (*vertices)[idx+1] = sint[j]*radf;
        (*vertices)[idx+2] = z;
        (*normals )[idx  ] = 0.f;
        (*normals )[idx+1] = 0.f;
        (*normals )[idx+2] = -1.f;
    }

    /* each stack */
    for (i=0; i<stacks+1; i++ )
    {
        for (j=0; j<slices; j++, idx+=3)
        {
            (*vertices)[idx  ] = cost[j]*radf;
            (*vertices)[idx+1] = sint[j]*radf;
            (*vertices)[idx+2] = z;
            (*normals )[idx  ] = cost[j];
            (*normals )[idx+1] = sint[j];
            (*normals )[idx+2] = 0.f;
        }

        z += zStep;
    }

    /* other on bottom (get normals right) */
    z -= zStep;
    for (j=0; j<slices; j++, idx+=3)
    {
        (*vertices)[idx  ] = cost[j]*radf;
        (*vertices)[idx+1] = sint[j]*radf;
        (*vertices)[idx+2] = z;
        (*normals )[idx  ] = 0.f;
        (*normals )[idx+1] = 0.f;
        (*normals )[idx+2] = 1.f;
    }

    /* bottom */
    (*vertices)[idx  ] =  0.f;
    (*vertices)[idx+1] =  0.f;
    (*vertices)[idx+2] =  height;
    (*normals )[idx  ] =  0.f;
    (*normals )[idx+1] =  0.f;
    (*normals )[idx+2] =  1.f;

    /* Release sin and cos tables */
    free(sint);
    free(cost);
}

void CircleTable(float **sint, float **cost, const int n, const bool halfCircle)
{
    int i;
    
    /* Table size, the sign of n flips the circle direction */
    const int size = abs(n);

    /* Determine the angle between samples */
    const float angle = (halfCircle?1:2)*(float)M_PI/(float)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */
    *sint = (float*)calloc(1, sizeof(float) * (size+1));
    *cost = (float*)calloc(1, sizeof(float) * (size+1));

    /* Bail out if memory allocation fails, fgError never returns */
    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        *sint=NULL;
        *cost=NULL;
        //fgError("Failed to allocate memory in fghCircleTable");
        return;
    }

    /* Compute cos and sin around the circle */
    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = (float)sin(angle*i);
        (*cost)[i] = (float)cos(angle*i);
    }

    
    if (halfCircle)
    {
        (*sint)[size] =  0.0f;  /* sin PI */
        (*cost)[size] = -1.0f;  /* cos PI */
    }
    else
    {
        /* Last sample is duplicate of the first (sin or cos of 2 PI) */
        (*sint)[size] = (*sint)[0];
        (*cost)[size] = (*cost)[0];
    }
}

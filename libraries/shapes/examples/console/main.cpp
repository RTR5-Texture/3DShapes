#include <stdio.h>
#include <stdlib.h>
#include "..\include\Aakaar.h"

void getCylinderVertices()
{
	float* cylinderVertices = NULL;
	int cylinder_vertices_count = 0;

	AakaarStatus status = GenerateCylinder(1.0f, 5.0f, 5, 5);
	if (status == AakaarStatus::AAKAAR_FAILED)
	{
		printf("\nFailed to get cube vertices.\n");
	}
	else {
		cylinder_vertices_count = GetCylinderVerticesCount();
		printf("\nFetched cylinder vertices count = %d\n", cylinder_vertices_count);
		GetCylinderVertices(&cylinderVertices);
		for (int i = 0; i < cylinder_vertices_count*3; i++)
		{
			printf("\n%f, %f, %f", cylinderVertices[i], cylinderVertices[i+1], cylinderVertices[i+2]);	
		}
	}
}

void GetCubeVertices()
{
	float* vertices = NULL;

	AakaarStatus status = GetCubeVertices(&vertices);
	if (status == AakaarStatus::AAKAAR_FAILED)
	{
		printf("\nFailed to get cube vertices.\n");
	}
	else {
		printf("\nFetched cube vertices successfully!!");
		for (int i = 0; i < NUM_CUBE_FACES; i++)
		{
			for (int j = 0; j < NUM_VERTICES_PER_CUBE_FACE; j = j + 3)
			{
				printf("\n%f, %f, %f", vertices[i * 12 + j], vertices[i * 12 + j + 1], vertices[i * 12 + j + 2]);
			}
			printf("\n\n");
		}
	}

	if (vertices)
	{
		free(vertices);
		vertices = NULL;
	}
}

void GetTorusVertices()
{
	int numVertices = 0;
	int numNormals = 0;
	VERTEX* vertices = NULL;
	NORMAL* normals = NULL;

	AakaarStatus status = GetTorusVertices(&vertices, &numVertices, &normals, &numNormals, 1.0f, 2.0f, 50, 100);
	if (status == AakaarStatus::AAKAAR_FAILED)
	{
		printf("\nFailed to get torus vertices and normals.\n");
	}
	else {
		printf("\nFetched torus vertices and normals successfully!!");

		printf("\n\n== Vertices ==\n\n");
		for (int i = 0; i < numVertices; i++)
		{
			printf("\n%f, %f, %f", vertices[i].x, vertices[i].y, vertices[i].z);
		}

		printf("\n\n== Normals ==\n\n");
		for (int i = 0; i < numNormals; i++)
		{
			printf("\n%f, %f, %f", normals[i].x, normals[i].y, normals[i].z);
		}

		printf("\n\n");
	}

	if (vertices)
	{
		free(vertices);
		vertices = NULL;
	}

	if (normals)
	{
		free(normals);
		normals = NULL;
	}
}


int main()
{
	int option = 0;

	do
	{
		printf("\n\n **** Aakaar Test Application ****\n\n");
		printf("\n\n1. Cube");
		printf("\n2. Torus");
		printf("\n3. Cylinder");
		printf("\n0. Exit");

		printf("\n\nEnter option: ");
		scanf("%d", &option);
		printf("\n\n");

		switch (option)
		{
			case 0:
				break;
			case 1:
				GetCubeVertices();
				break;
			case 2:
				GetTorusVertices();
				break;
			case 3:
				getCylinderVertices();
				break;
			default:
				break;
		}

	} while (option != 0);


	printf("\n\n");
	return 0;
}
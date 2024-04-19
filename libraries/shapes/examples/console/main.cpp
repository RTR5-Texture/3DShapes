#include <stdio.h>
#include "..\include\Aakaar.h"

int main()
{
	float* cubeVertices = NULL;

	AakaarStatus status = GetCubeVertices(&cubeVertices);
	if (status == AAKAAR_FAILED)
	{
		printf("\nFailed to get cube vertices.\n");
	}
	else {
		printf("\nFetched cube vertices successfully!!");
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 12; j = j + 3)
			{
				printf("\n%f, %f, %f", cubeVertices[i * 12 + j], cubeVertices[i * 12 + j + 1], cubeVertices[i * 12 + j + 2]);
			}
			printf("\n\n");
		}
	}

	if (cubeVertices)
	{
		free(cubeVertices);
		cubeVertices = NULL;
	}

	printf("\n\n");
	return 0;
}
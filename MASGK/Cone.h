#pragma once

#include<math.h>
#include"Lib3d.h"
#include"Mesh.h"

#define TWOPI 6.28318530718

using namespace std;

struct Cone : Mesh
{
	Cone(int walls)
	{
		vSize = walls + 2;
		tSize = walls * 2;
		verts = new Vertex[vSize];
		ids = new int3[tSize];

		verts[0].p = float3(0, 1, 0);
		verts[1].p = float3(0, 0, 0);
		verts[0].c = float3(1,1,1);
		verts[1].c = float3(1,1,1);

		// setup verts
		float progress;
		for (int i = 0; i < walls; ++i)
		{
			progress = (float)i / walls * TWOPI;
			verts[i + 2].p = float3(sin(progress) * 0.5, 0, cos(progress) * 0.5);
			verts[i + 2].c = float3(i % 3 == 0, i % 3 == 1, i % 3 == 2);
		}

		// setup tris
		for (int i = 0; i < walls; ++i)
		{
			progress = (float)i / walls * TWOPI;

			//top
			ids[i * 2].a = 0;
			ids[i * 2].b = (i + 2) % vSize;
			ids[i * 2].c = (i + 3) % vSize;

			//bottom
			ids[i * 2 + 1].a = 1;
			ids[i * 2 + 1].b = (i + 3) % vSize;
			ids[i * 2 + 1].c = (i + 2) % vSize;

			if (ids[i * 2].b < 2) ids[i * 2].b += 2;
			if (ids[i * 2].c < 2) ids[i * 2].c += 2;
			if (ids[i * 2 + 1].b < 2) ids[i * 2 + 1].b += 2;
			if (ids[i * 2 + 1].c < 2) ids[i * 2 + 1].c += 2;
		}

		CalcNormals();
	}
};
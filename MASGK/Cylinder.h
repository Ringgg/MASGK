#pragma once

#include<math.h>
#include"Lib3d.h"
#include"Mesh.h"

#define TWOPI 6.28318530718

using namespace std;

struct Cylinder : Mesh
{
	Cylinder(int walls)
	{
		vSize = 2 * walls + 4;
		tSize = 4 * walls;
		verts = new Vertex[vSize];
		ids = new int3[tSize];

		verts[0].p = float3(0, 0.5, 0);
		verts[1].p = float3(0, -0.5, 0);

		// setup verts
		float progress;
		for (int i = 0; i < walls; ++i)
		{
			progress = (float)i / walls * TWOPI;
			verts[i*2 + 3].p = float3(sin(progress),  0.5, cos(progress));
			verts[i*2 + 4].p = float3(sin(progress), -0.5, cos(progress));

			verts[i * 2 + 3].c = verts[i * 2 + 4].c = float3(progress, progress, progress);
		}

		// setup tris
		for (int i = 0; i < walls; ++i)
		{
			progress = (float)i / walls * TWOPI;

			//top
			ids[i * 4].a = 0;
			ids[i * 4].b = (i * 2 + 3) % vSize;
			ids[i * 4].c = (i * 2 + 5) % vSize;

			//bottom
			ids[i * 4 + 1].a = 1;
			ids[i * 4 + 1].b = (i * 2 + 4) % vSize;
			ids[i * 4 + 1].c = (i * 2 + 6) % vSize;

			//wall 1
			ids[i * 4 + 2].a = (i * 2 + 3) % vSize;
			ids[i * 4 + 2].b = (i * 2 + 4) % vSize;
			ids[i * 4 + 2].c = (i * 2 + 5) % vSize;

			//wall 2
			ids[i * 4 + 3].a = (i * 2 + 4) % vSize;
			ids[i * 4 + 3].b = (i * 2 + 6) % vSize;
			ids[i * 4 + 3].c = (i * 2 + 5) % vSize;
		}

		CalcNormals();
	}
};
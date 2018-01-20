#pragma once

#include"Lib3d.h"
#include"Mesh.h"

struct Cube : Mesh
{
	Cube()
	{
		vSize = 8;
		tSize = 12;
		verts = new Vertex[vSize];
		ids = new int3[tSize];


		verts[0].p = float3(-0.500000, -0.500000, 0.500000);
		verts[1].p = float3(0.500000, -0.500000, 0.500000);
		verts[2].p = float3(-0.500000, 0.500000, 0.500000);
		verts[3].p = float3(0.500000, 0.500000, 0.500000);
		verts[4].p = float3(-0.500000, 0.500000, -0.500000);
		verts[5].p = float3(0.500000, 0.500000, -0.500000);
		verts[6].p = float3(- 0.500000, - 0.500000, - 0.500000);
		verts[7].p = float3(0.500000, -0.500000, -0.500000);

		verts[0].c = float3(1, 0, 0);
		verts[1].c = float3(0, 1, 0);
		verts[2].c = float3(0, 0, 1);
		verts[3].c = float3(1, 0, 0);
		verts[4].c = float3(0, 1, 0);
		verts[5].c = float3(0, 0, 1);
		verts[6].c = float3(1, 0, 0);
		verts[7].c = float3(0, 1, 0);
		
		ids[0] =  int3(0, 1, 2);
		ids[1] =  int3(2, 1, 3);
		ids[2] =  int3(2, 3, 4);
		ids[3] =  int3(4, 3, 5);
		ids[4] =  int3(4, 5, 6);
		ids[5] =  int3(6, 5, 7);
		ids[6] =  int3(6, 7, 0);
		ids[7] =  int3(0, 7, 1);
		ids[8] =  int3(1, 7, 3);
		ids[9] =  int3(3, 7, 5);
		ids[10] = int3(6, 0, 4);
		ids[11] = int3(4, 0, 2);
		
		verts[0].n = verts[0].p.Normalized(); // {0, 1, 0, 0};//
		verts[1].n = verts[1].p.Normalized(); // {0, 1, 0, 0};//
		verts[2].n = verts[2].p.Normalized(); // {0, 1, 0, 0};//
		verts[3].n = verts[3].p.Normalized(); // {0, 1, 0, 0};//
		verts[4].n = verts[4].p.Normalized(); // {0, 1, 0, 0};//
		verts[5].n = verts[5].p.Normalized(); // {0, 1, 0, 0};//
		verts[6].n = verts[6].p.Normalized(); // {0, 1, 0, 0};//
		verts[7].n = verts[7].p.Normalized(); // {0, 1, 0, 0};//
	}
};
#pragma once

#include"Lib3d.h"
#include"Mesh.h"

struct SimpleTriangle : Mesh
{
	SimpleTriangle()
	{
		vSize = 3;
		tSize = 1;
		verts = new Vertex[3];
		ids = new int3[1];
		
		verts[0].p = float3(-0.5, 0, 0);
		verts[1].p = float3(0, 0.5, 0);
		verts[2].p = float3(0.5, 0, 0);
		
		verts[0].c = float3(1, 0, 0);
		verts[1].c = float3(0, 1, 0);
		verts[2].c = float3(0, 0, 1);
		
		ids[0] = int3(0, 1, 2);
		
		CalcNormals();
	}
};
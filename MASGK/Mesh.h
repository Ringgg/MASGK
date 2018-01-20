#pragma once

struct Mesh;

#include "Lib3d.h"
#include "VertexProcessor.h"
#include "Rasterizer.h"

struct Mesh
{
	int vSize, tSize;
	Vertex* verts;
	int3* ids;

	void Draw(Rasterizer& r, VertexProcessor& vp);
	void CalcNormals();
};
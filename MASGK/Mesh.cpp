#include "Mesh.h"

void Mesh::Draw(Rasterizer& r, VertexProcessor& vp)
{
	for (int i = 0; i < vSize; ++i)
		vp.Tr(verts[i]);

	for (int i = 0; i < tSize; ++i)
		r.Triangle(
			verts[ids[i].a],
			verts[ids[i].b],
			verts[ids[i].c]);
}

void Mesh::CalcNormals()
{
	float3 n;
	int i = vSize;
	while (i--> 0)
		verts[i].n = float3(0, 0, 0);

	i = tSize;
	while (i--> 0)
	{
		n = Linear::Cross(verts[ids[i].c].p - verts[ids[i].a].p, verts[ids[i].c].p - verts[ids[i].b].p);
		n.Normalize();

		verts[ids[i].a].n += n;
		verts[ids[i].b].n += n;
		verts[ids[i].c].n += n;
	}

	i = vSize;
	while (i--> 0)
		verts[i].n.Normalize();
}

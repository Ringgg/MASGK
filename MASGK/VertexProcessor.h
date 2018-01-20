#pragma once

struct VertexProcessor;

#include <math.h>
#include <vector>
#include"Lib3d.h"
#include"Light.h"

using namespace std;

struct VertexProcessor
{
	VertexProcessor();
	~VertexProcessor();

	float4x4 M, V, P, MV, MVP, MVT;

	void Tr(Vertex& v);

	void SetPerspective(float fovy, float aspect, float near, float far);
	void SetLookAt(float3 eye, float3 center, float3 up);
	void SetIdentity();
	void MultByTrans(float3 v);
	void MultByScale(float3 v);
	void MultByRot(float a, float3 v);

	vector<Light*> lights;
private:

	bool dirty = true;
	void RecalcMatrix();
};
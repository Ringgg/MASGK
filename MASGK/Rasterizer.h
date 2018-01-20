#pragma once

class Rasterizer;

#include <vector>
#include <algorithm>
#include"Lib3d.h"
#include"Light.h"
#include"VertexProcessor.h"

using namespace std;

class Rasterizer
{
public:
	Rasterizer(Buffer& buffer);
	virtual ~Rasterizer();

	vector<Light*> lights;

	u32 ToArgb(float3& c);
	void Triangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);
	void GetColor(const Vertex& a, const Vertex& b, const Vertex& c);

private:
	Buffer& buffer;

	//helpers
	float3 lambda, uv, f;
	float4 pos, norm;
	double x, y, minX, maxX, minY, maxY, stepX, stepY;
	double dx12, dx23, dx31, dx13, dx32, dy12, dy23, dy31;
	float lambda1den, lambda2den, depth;
	bool topLeft1, topLeft2, topLeft3;
	int idX, idY;

	// light
	float3 ambient, diffuse, specular, finalColor;
	//float3 lightVec, viewVec, halfVec;
	//float intensity, LdotN, NdotH;
};
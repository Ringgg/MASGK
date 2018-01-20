#pragma once

class Rasterizer;

#include <vector>
#include <algorithm>
#include <ctime>
#include <xmmintrin.h>
#include <iostream>

#include"Lib3d.h"
#include"Light.h"
#include"VertexProcessor.h"

using namespace std;

class Rasterizer
{
public:
	double time_ps;

	Rasterizer(Buffer& buffer);
	virtual ~Rasterizer();

	vector<Light*> lights;

	u32 ToArgb(float3& c);
	void Triangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);
	void GetColor(const Vertex& a, const Vertex& b, const Vertex& c);

private:
	Buffer& buffer;

	//helpers
	float3 lambda, uv, f, n;
	float4 pos, norm;
	double x, y, minX, maxX, minY, maxY, stepX, stepY;
	double dx12, dx23, dx31, dx13, dx32, dy12, dy23, dy31;
	float lambda1den, lambda2den, depth, maxWidth, maxHeight;
	bool topLeft1, topLeft2, topLeft3;
	int idX, idY, x1, x2, x3, y1, y2, y3, xx, yy;

	// light
	float4 lightVec, viewVec, halfVec;
	float3 finalColor, colorMult, tmpCol;
	float attenuation, LdotN, NdotH;

	const float eps = -std::numeric_limits<float>::epsilon();
};
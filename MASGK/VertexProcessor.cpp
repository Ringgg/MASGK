#include "VertexProcessor.h"


VertexProcessor::VertexProcessor()
{
	SetIdentity();
}

VertexProcessor::~VertexProcessor() { }

void VertexProcessor::Tr(Vertex& v)
{
	if (dirty) RecalcMatrix();

	v.p2 = Linear::Mult(v.p, MV);
	v.p = Linear::Mult(v.p, MVP);
	v.n = Linear::Mult(v.n, MVT);
	
	v.p *= 1.0f / v.p.w;
}


void VertexProcessor::RecalcMatrix()
{
	MV = Linear::Mult(V, M);
	MVP = Linear::Mult(P, MV);

	MVT = Linear::inverse(MV);
	Linear::Transpose(MVT);

	float4x4 VT = V;
	VT = Linear::inverse(VT);
	Linear::Transpose(VT);
	
	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i]->transOrig = Linear::Mult(lights[i]->orig, V);
		lights[i]->transDir = Linear::Mult(lights[i]->dir, VT);
	}

	dirty = false;
}

void VertexProcessor::SetPerspective(float fovy, float aspect, float near, float far)
{
	fovy *= 3.141592 / 360;
	float f = cos(fovy) / sin(fovy);

	P[0] = float4(f / aspect, 0, 0, 0);
	P[1] = float4(0, f, 0, 0);
	P[2] = float4(0, 0, (far+near)/(near-far), -1);
	P[3] = float4(0, 0, 2*far*near/(near-far), 0);
	dirty = true;
}

void VertexProcessor::SetLookAt(float3 eye, float3 center, float3 up)
{
	float3 f = (center - eye).Normalized();
	up.Normalize();
	float3 s = Linear::Cross(f, up);
	float3 u = Linear::Cross(s, f);

	V[0] = float4(s[0], u[0], -f[0], 0);
	V[1] = float4(s[1], u[1], -f[1], 0);
	V[2] = float4(s[2], u[2], -f[2], 0);
	V[3] = float4
	(
		-Linear::Dot(s, eye),
		-Linear::Dot(u, eye), 
		-Linear::Dot(f, eye), 
		1
	);
	dirty = true;
}

void VertexProcessor::SetIdentity()
{
	M = float4x4(
		float4(1, 0, 0, 0),
		float4(0, 1, 0, 0),
		float4(0, 0, 1, 0),
		float4(0, 0, 0, 1));
	dirty = true;
}

void VertexProcessor::MultByTrans(float3 v)
{
	float4x4 m(
		float4(1,   0,   0,   0),
		float4(0,   1,   0,   0),
		float4(0,   0,   1,   0),
		float4(v.x, v.y, v.z, 1));
	M = Linear::Mult(m, M);
	dirty = true;
}

void VertexProcessor::MultByScale(float3 v)
{
	float4x4 m(
		float4(v.x, 0, 0, 0),
		float4(0, v.y, 0, 0),
		float4(0, 0, v.z, 0),
		float4(0, 0, 0,   1));
	M = Linear::Mult(m, M);
	dirty = true;
}

void VertexProcessor::MultByRot(float a, float3 v)
{
	float s = sin(a*3.141592 / 180);
	float c = cos(a*3.141592 / 180);
	v.Normalize();
	float4x4 m(
		float4(v.x*v.x*(1 - c) + c, v.y*v.x*(1 - c) + v.z*s, v.x*v.z*(1 - c) - v.y*s, 0),
		float4(v.x*v.y*(1 - c) - v.z*s, v.y*v.y*(1 - c) + c, v.y*v.z*(1 - c) + v.x*s, 0),
		float4(v.x*v.z*(1 - c) + v.y*s, v.y*v.z*(1 - c) - v.x*s, v.z*v.z*(1 - c) + c, 0),
		float4(0, 0, 0, 1));

	M = Linear::Mult(m, M);
	dirty = true;
}
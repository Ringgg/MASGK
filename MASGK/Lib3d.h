#include "typedefs.h"
#include "Buffer.h"
#include "int3.h"
#include "float3.h"
#include "float4.h"
#include "float4x4.h"
#include "Linear.h"

#ifndef VERTEX
#define VERTEX

struct Vertex
{
	float4 p, p2;
	float4 n;
	float3 uv;
	float3 c;
};

#endif // !VERTEX

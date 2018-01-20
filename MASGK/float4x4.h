#pragma once

struct float4x4;

#include <math.h>
#include "Float4.h"

struct float4x4
{
    float4 fields[4];

    float4x4(): fields{
        float4(0,0,0,0), float4(0,0,0,0),
        float4(0,0,0,0), float4(0,0,0,0)} {}

    float4x4(const float4& a, const float4& b, const float4& c, const float4& d): fields{ a, b, c, d } {}
	float4x4(const float4x4& a): fields{a[0], a[1], a[2], a[3] } {}
	
	float4& operator[](const int i) const { return *((float4*)this + i); }
};
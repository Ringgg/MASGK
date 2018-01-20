#include "float4.h"

float4::float4() : m() { w = 1; }
float4::float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
float4::float4(const float4& orig) : m(orig.m) { }
float4::float4(const float3& orig) : m(orig.m) { w = 1; }
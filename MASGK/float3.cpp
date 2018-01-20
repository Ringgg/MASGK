#include "float3.h"

float3::float3() : m() {}
float3::float3(float x, float y, float z) : x(x), y(y), z(z) {}
float3::float3(const float3& orig) : m(orig.m) { }
float3::float3(const float4& orig) : m(orig.m) { }
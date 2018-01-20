#include "float3.h"

float3::float3() : x(0), y(0), z(0) {}
float3::float3(float x, float y, float z) : x(x), y(y), z(z) {}
float3::float3(const float3& orig) : x(orig.x), y(orig.y), z(orig.z) { }
float3::float3(const float4& orig) : x(orig.x), y(orig.y), z(orig.z) { }
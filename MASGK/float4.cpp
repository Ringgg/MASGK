#include "float4.h"

float4::float4() : x(0), y(0), z(0), w(1) { }
float4::float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
float4::float4(const float4& orig) : x(orig.x), y(orig.y), z(orig.z), w(orig.w) { }
float4::float4(const float3& orig) : x(orig.x), y(orig.y), z(orig.z), w(1) { }
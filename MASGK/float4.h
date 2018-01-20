#pragma once

#include<math.h>
#include <xmmintrin.h>

union float4;
#include"float3.h"

union float4
{
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	struct { __m128 m; };

	float4();
	float4(float x, float y, float z, float w);
	float4(const float4& orig);
	float4(const float3& orig);

    float& operator[](int i)          const { return  *((float*)this + i);                             }
    float4 operator+(const float4& r) const { return float4(x + r.x, y + r.y, z + r.z, w + r.w);       }
    float4 operator-(const float4& r) const { return float4(x - r.x, y - r.y, z - r.z, w - r.w);       }
    float4 operator-()                const { return float4(-x, -y, -z, -w);                           }
	float4 operator*(const float r)   const { return float4(x * r, y * r, z * r, w * r); }
	float4 operator/(const float r)   const { return float4(x / r, y / r, z / r, w / r); }
    float4& operator+=(const float4& r)        { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
    float4& operator-=(const float4& r)        { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
    float4& operator*=(const float4& r)        { x *= r.x; y *= r.y; z *= r.z; w *= r.w; return *this; }
    float4& operator*=(const float r)          { x *= r;   y *= r;   z *= r;   w += r;   return *this; }

    void Normalize()
    {
        static const float epsilon = 0.000001f;
        float len = sqrt(x*x + y*y + z*z + w*w);
        if (len > epsilon){ x /= len; y /= len; z /=len; w /= len; }
        else x = y = z = w = 0.0f;
    }

	float4 Normalized() const
	{
		static const float epsilon = 0.000001f;
		float len = sqrt(x*x + y*y + z*z + w*w);
		if (len > epsilon) return float4(x / len, y / len, z / len, w / len);
		else return float4(0, 0, 0, 0);
	}

    float sqLen() const { return x*x + y*y + z*z + w*w;       }
    float len()   const { return sqrt(x*x + y*y + z*z + w*w); }
};

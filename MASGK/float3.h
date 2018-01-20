#pragma once

#include <math.h>

union float3;
#include "float4.h"

__declspec(align(32))
union float3
{
	struct { float x, y, z, align; };
	struct { float r, g, b, align; };
	struct { __m128 m; };

	float3();
	float3(float x, float y, float z);
	float3(const float3& orig);
	float3(const float4& orig);
	float3(const __m128& m) : m(m) { }

    float& operator[](const int i)    const { return *((float*)this + i);                 }
    float3 operator+(const float3& r) const { return float3(_mm_add_ps(m, r.m));          }
    float3 operator-(const float3& r) const { return float3(_mm_sub_ps(m, r.m));          }
    float3 operator-()                const { return float3(-x, -y, -z);                  }
    float3 operator*(float r)         const { return float3(x * r, y * r, z * r);         }
	float3 operator/(const float r)   const { return float3(x / r, y / r, z / r);		  }
	float3 operator*(const float3& r) const { return float3(_mm_mul_ps(m, r.m));		  }
    float3& operator+=(const float3& r)     { m = _mm_add_ps(m, r.m); return *this;       }
    float3& operator-=(const float3& r)     { m = _mm_sub_ps(m, r.m); return *this;       }
    float3& operator*=(const float3& r)     { m = _mm_mul_ps(m, r.m); return *this;       }
    float3& operator*=(const float r)       { x *= r;   y *= r;   z *= r;   return *this; }

    void Normalize()
    {
        static const float epsilon = 0.000001f;
        float len = sqrt(x*x + y*y + z*z);
        if (len > epsilon){ x /= len; y /= len; z /=len; }
        else x = y = z = 0.0f;
    }

	float3 Normalized() const
	{
		static const float epsilon = 0.000001f;
		float len = sqrt(x*x + y*y + z*z);
		if (len > epsilon) return float3(x / len, y / len, z / len);
		else return float3(0, 0, 0);
	}

    float sqLen() const { return x*x + y*y + z*z;       }
    float len()   const { return sqrt(x*x + y*y + z*z); }
};
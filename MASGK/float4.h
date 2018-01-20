#pragma once

#include<math.h>
#include <xmmintrin.h>
#include <smmintrin.h>

union float4;
#include"float3.h"

__declspec(align(32))
union float4
{
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	struct { __m128 m; };

	float4();
	float4(float x, float y, float z, float w);
	float4(const float4& orig);
	float4(const float3& orig);
	float4(const __m128& m) : m(m) { }

    inline float& operator[](int i)          const { return  *((float*)this + i);                  }
    inline float4 operator+(const float4& r) const { return float4(_mm_add_ps(m, r.m));            }
    inline float4 operator-(const float4& r) const { return float4(_mm_sub_ps(m, r.m));            }
    inline float4 operator-()                const { return float4(-x, -y, -z, -w);                }
	inline float4 operator*(const float r)   const { return float4(x * r, y * r, z * r, w * r);    }
	inline float4 operator/(const float r)   const { return float4(x / r, y / r, z / r, w / r);    }
	inline float4 operator*(const float4& r) const { return float4(_mm_mul_ps(m, r.m));			}
    inline float4& operator+=(const float4& r)     { m = _mm_add_ps(m, r.m); return *this;         }
    inline float4& operator-=(const float4& r)     { m = _mm_sub_ps(m, r.m); return *this;         }
    inline float4& operator*=(const float4& r)     { m = _mm_mul_ps(m, r.m); return *this;         }
    inline float4& operator*=(const float r)       { x *= r; y *= r; z *= r; w *= r; return *this; }

    //inline void Normalize()
    //{
    //    static const float epsilon = 0.000001f;
	//	static float len;
	//
    //    len = sqrt(x*x + y*y + z*z + w*w);
    //    if (len > epsilon){ x /= len; y /= len; z /=len; w /= len; }
    //    else x = y = z = w = 0.0f;
    //}

	inline void Normalize()
	{
		static __m128 dp;
	
		dp = _mm_dp_ps(m, m, 0b11111111);
		dp = _mm_rsqrt_ps(dp);
		m = _mm_mul_ps(m, dp);
	}

	inline float4 Normalized() const
	{
		static const float epsilon = 0.000001f;
		static float len;

		len = sqrt(x*x + y*y + z*z + w*w);
		if (len > epsilon) return float4(x / len, y / len, z / len, w / len);
		else return float4(0, 0, 0, 0);
	}

    inline float sqLen() const { return x*x + y*y + z*z + w*w;       }
    inline float len()   const { return sqrt(x*x + y*y + z*z + w*w); }
};

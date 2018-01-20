#pragma once

#include <smmintrin.h>

#include "float3.h";
#include "float4.h";
#include "float4x4.h";
#include "int3.h";

struct Linear
{
	static inline void ADD(const float4 &a, const float4 &b, float4 &c) { c.m = _mm_add_ps(a.m, b.m); }
	static inline void ADD(const float3 &a, const float3 &b, float3 &c) { c.m = _mm_add_ps(a.m, b.m); }
	static inline void SUB(const float4 &a, const float4 &b, float4 &c) { c.m = _mm_sub_ps(a.m, b.m); }
	static inline void SUB(const float3 &a, const float3 &b, float3 &c) { c.m = _mm_sub_ps(a.m, b.m); }
	static inline void MUL(const float4 &a, const float4 &b, float4 &c) { c.m = _mm_mul_ps(a.m, b.m); }
	static inline void MUL(const float3 &a, const float3 &b, float3 &c) { c.m = _mm_mul_ps(a.m, b.m); }

	static inline float3 Cross(const float3& a, const float3& b)
	{
		return float3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	static inline float Dot(const float3& a, const float3& b)
	{
		static __m128 r1, r2, r3;
		static float result;
		r1 = _mm_mul_ps(a.m, b.m);
		r2 = _mm_hadd_ps(r1, r1);
		r3 = _mm_hadd_ps(r2, r2);
		_mm_store_ss(&result, r3);

		return result;
	}

	static inline float Dot(const float4& a, const float4& b)
	{
		static __m128 r1, r2, r3;
		static float result;
		r1 = _mm_mul_ps(a.m, b.m);
		r2 = _mm_hadd_ps(r1, r1);
		r3 = _mm_hadd_ps(r2, r2);
		_mm_store_ss(&result, r3);

		return result;
	}

	static float4x4 Mult(const float4x4& a, const float4x4& b)
	{
		float4x4 result{ { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } };

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				for(int k = 0; k < 4; k++)
					result[i][j] += a[k][j] * b[i][k];

		return result;
	}
	
	static float4 Mult(const float4& a, const float4x4& b)
	{
		return float4
		(
			b[0][0] * a[0] + b[1][0] * a[1] + b[2][0] * a[2] + b[3][0] * a[3],
			b[0][1] * a[0] + b[1][1] * a[1] + b[2][1] * a[2] + b[3][1] * a[3],
			b[0][2] * a[0] + b[1][2] * a[1] + b[2][2] * a[2] + b[3][2] * a[3],
			b[0][3] * a[0] + b[1][3] * a[1] + b[2][3] * a[2] + b[3][3] * a[3]
		);
	}

	static void Transpose(float4x4& a)
	{
		static float tmp;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = i + 1; j < 4; ++j)
			{
				if (i == j) continue;
				tmp = a[i][j];
				a[i][j] = a[j][i];
				a[j][i] = tmp;
			}
		}
	}

	static void Saturate(float3& a)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (a[i] < 0) a[i] = 0;
			else if (a[i] > 1) a[i] = 1;
		}
	}

	static float3 Saturated(const float3& a)
	{
		return float3((a.x < 0 ? 0 : (a.x > 1 ? 1 : a.x)),
					  (a.y < 0 ? 0 : (a.y > 1 ? 1 : a.y)),
					  (a.z < 0 ? 0 : (a.z > 1 ? 1 : a.z)));
	}

	static float4x4 inverse(float4x4 mat)
	{
		float4x4 inv;

		inv[0][0] = mat[1][1] * mat[2][2] * mat[3][3] - mat[1][1] * mat[2][3] * mat[3][2] - mat[2][1] * mat[1][2] * mat[3][3] +
					mat[2][1] * mat[1][3] * mat[3][2] + mat[3][1] * mat[1][2] * mat[2][3] - mat[3][1] * mat[1][3] * mat[2][2];

		inv[1][0] = -mat[1][0] * mat[2][2] * mat[3][3] + mat[1][0] * mat[2][3] * mat[3][2] + mat[2][0] * mat[1][2] * mat[3][3] -
					mat[2][0] * mat[1][3] * mat[3][2] - mat[3][0] * mat[1][2] * mat[2][3] + mat[3][0] * mat[1][3] * mat[2][2];

		inv[2][0] = mat[1][0] * mat[2][1] * mat[3][3] - mat[1][0] * mat[2][3] * mat[3][1] - mat[2][0] * mat[1][1] * mat[3][3] +
					mat[2][0] * mat[1][3] * mat[3][1] + mat[3][0] * mat[1][1] * mat[2][3] - mat[3][0] * mat[1][3] * mat[2][1];

		inv[3][0] = -mat[1][0] * mat[2][1] * mat[3][2] + mat[1][0] * mat[2][2] * mat[3][1] + mat[2][0] * mat[1][1] * mat[3][2] -
					mat[2][0] * mat[1][2] * mat[3][1] - mat[3][0] * mat[1][1] * mat[2][2] + mat[3][0] * mat[1][2] * mat[2][1];

		inv[0][1] = -mat[0][1] * mat[2][2] * mat[3][3] + mat[0][1] * mat[2][3] * mat[3][2] + mat[2][1] * mat[0][2] * mat[3][3] -
					mat[2][1] * mat[0][3] * mat[3][2] - mat[3][1] * mat[0][2] * mat[2][3] + mat[3][1] * mat[0][3] * mat[2][2];

		inv[1][1] = mat[0][0] * mat[2][2] * mat[3][3] - mat[0][0] * mat[2][3] * mat[3][2] - mat[2][0] * mat[0][2] * mat[3][3] +
					mat[2][0] * mat[0][3] * mat[3][2] + mat[3][0] * mat[0][2] * mat[2][3] - mat[3][0] * mat[0][3] * mat[2][2];

		inv[2][1] = -mat[0][0] * mat[2][1] * mat[3][3] + mat[0][0] * mat[2][3] * mat[3][1] + mat[2][0] * mat[0][1] * mat[3][3] -
					mat[2][0] * mat[0][3] * mat[3][1] - mat[3][0] * mat[0][1] * mat[2][3] + mat[3][0] * mat[0][3] * mat[2][1];

		inv[3][1] = mat[0][0] * mat[2][1] * mat[3][2] - mat[0][0] * mat[2][2] * mat[3][1] - mat[2][0] * mat[0][1] * mat[3][2] +
					mat[2][0] * mat[0][2] * mat[3][1] + mat[3][0] * mat[0][1] * mat[2][2] - mat[3][0] * mat[0][2] * mat[2][1];

		inv[0][2] = mat[0][1] * mat[1][2] * mat[3][3] - mat[0][1] * mat[1][3] * mat[3][2] - mat[1][1] * mat[0][2] * mat[3][3] +
					mat[1][1] * mat[0][3] * mat[3][2] + mat[3][1] * mat[0][2] * mat[1][3] - mat[3][1] * mat[0][3] * mat[1][2];

		inv[1][2] = -mat[0][0] * mat[1][2] * mat[3][3] + mat[0][0] * mat[1][3] * mat[3][2] + mat[1][0] * mat[0][2] * mat[3][3] -
					mat[1][0] * mat[0][3] * mat[3][2] - mat[3][0] * mat[0][2] * mat[1][3] + mat[3][0] * mat[0][3] * mat[1][2];

		inv[2][2] = mat[0][0] * mat[1][1] * mat[3][3] - mat[0][0] * mat[1][3] * mat[3][1] - mat[1][0] * mat[0][1] * mat[3][3] +
					mat[1][0] * mat[0][3] * mat[3][1] + mat[3][0] * mat[0][1] * mat[1][3] - mat[3][0] * mat[0][3] * mat[1][1];

		inv[3][2] = -mat[0][0] * mat[1][1] * mat[3][2] + mat[0][0] * mat[1][2] * mat[3][1] + mat[1][0] * mat[0][1] * mat[3][2] -
					mat[1][0] * mat[0][2] * mat[3][1] - mat[3][0] * mat[0][1] * mat[1][2] + mat[3][0] * mat[0][2] * mat[1][1];

		inv[0][3] = -mat[0][1] * mat[1][2] * mat[2][3] + mat[0][1] * mat[1][3] * mat[2][2] + mat[1][1] * mat[0][2] * mat[2][3] -
					mat[1][1] * mat[0][3] * mat[2][2] - mat[2][1] * mat[0][2] * mat[1][3] + mat[2][1] * mat[0][3] * mat[1][2];

		inv[1][3] = mat[0][0] * mat[1][2] * mat[2][3] - mat[0][0] * mat[1][3] * mat[2][2] - mat[1][0] * mat[0][2] * mat[2][3] +
					mat[1][0] * mat[0][3] * mat[2][2] + mat[2][0] * mat[0][2] * mat[1][3] - mat[2][0] * mat[0][3] * mat[1][2];

		inv[2][3] = -mat[0][0] * mat[1][1] * mat[2][3] + mat[0][0] * mat[1][3] * mat[2][1] + mat[1][0] * mat[0][1] * mat[2][3] -
					mat[1][0] * mat[0][3] * mat[2][1] - mat[2][0] * mat[0][1] * mat[1][3] + mat[2][0] * mat[0][3] * mat[1][1];

		inv[3][3] = mat[0][0] * mat[1][1] * mat[2][2] - mat[0][0] * mat[1][2] * mat[2][1] - mat[1][0] * mat[0][1] * mat[2][2] +
					mat[1][0] * mat[0][2] * mat[2][1] + mat[2][0] * mat[0][1] * mat[1][2] - mat[2][0] * mat[0][2] * mat[1][1];

		float det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0] + mat[0][3] * inv[3][0];

		if (det == 0)
			return inv;

		det = 1.0 / det;

		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				inv[x][y] *= det;

		return inv;
	}

	static void Project(float3& a, const float3& ontoB)
	{
		a = ontoB * Dot(a, ontoB);
	}

	static float3 Projected(const float3& a, const float3& ontoB)
	{
		return  ontoB * Dot(a, ontoB);
	}

	static void Reflect(float3& a, const float3& offB)
	{
		a -= Projected(a, offB) * 2;
	}

	static float3 Reflected(const float3& a, const float3& offB)
	{
		return a - Projected(a, offB) * 2;
	}	
};
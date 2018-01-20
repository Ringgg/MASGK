#pragma once
#include "Lib3d.h"
#include <math.h>

struct Light
{
	Light(float4 orig, float4 dir) : orig(orig), dir(dir.Normalized()) { }

	// orig.w == 0 -> directional
	// orig.w == 1 -> point
	float4 orig;
	float4 dir;
	float3 ambient;
	float3 diffuse;
	float3 specular;

	float4 transOrig;
	float4 transDir;

	// cutoff == 0 -> point light, else cone
	float cutoff = 0.0f;
	float exp = 1.0f;
	float attenuation = 0.0f;

private:
	float intensity, DdotVangle;

public:
	float4 GetDir(float4 toPt)
	{
		if (orig.w == 0) return  transDir;
		if (orig.w == 1) return  (transOrig - toPt);
	}


	float GetIntensity(float4 vec) // point in space illumination
	{
		if (orig.w == 0)
			return 1.0f;

		intensity = 1.0f / (1.0f + (vec.sqLen() * attenuation));

		if (cutoff == 0)
			return intensity;

		DdotVangle = Linear::Dot(transDir.Normalized(), -vec.Normalized());

		if (DdotVangle < (90 - cutoff) * 3.1415 / 180)
			return 0.0f;

		return fmax(0.0f, std::pow(DdotVangle, exp) * intensity);
	}
};
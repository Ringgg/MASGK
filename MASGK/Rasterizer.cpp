#include "Rasterizer.h"

Rasterizer::Rasterizer(Buffer& buffer) : buffer(buffer) { }
Rasterizer::~Rasterizer() { }

u32 Rasterizer::ToArgb(float3& c)
{
	return
		  ((u32)((u8)(c.x * 256)) << 0 )
		+ ((u32)((u8)(c.y * 256)) << 8 )
		+ ((u32)((u8)(c.z * 256)) << 16)
		+ ((u32)256               << 24);
}

void Rasterizer::Triangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	// a.p <- screenspace coords
	// a.p2 <- base point * MV matrix

	n = Linear::Cross(
		float3(c.p.x - a.p.x, c.p.y - a.p.y, 0),
		float3(c.p.x - b.p.x, c.p.y - b.p.y, 0));
	if (n.z >= 0)
		return;

	dx12 = a.p.x - b.p.x;    dx23 = b.p.x - c.p.x;    dx31 = c.p.x - a.p.x;
	dy12 = a.p.y - b.p.y;    dy23 = b.p.y - c.p.y;    dy31 = c.p.y - a.p.y;
	dx13 = a.p.x - c.p.x;    dx32 = c.p.x - b.p.x;

	lambda1den = 1 / (dy23*dx13 + dx32*(a.p.y - c.p.y));
	lambda2den = 1 / (dy31*dx23 + dx13*(dy23));

	topLeft1 = dy12 < 0 || dy12 == 0 && dx12 > 0;
	topLeft2 = dy23 < 0 || dy23 == 0 && dx23 > 0;
	topLeft3 = dy31 < 0 || dy31 == 0 && dx31 > 0;

	maxWidth = buffer.w - 1.0F;
	maxHeight = buffer.h - 1.0F;

	x1 = (a.p.x + 1.0F) * maxWidth * 0.5F;
	x2 = (b.p.x + 1.0F) * maxWidth * 0.5F;
	x3 = (c.p.x + 1.0F) * maxWidth * 0.5F;

	y1 = (a.p.y + 1.0F) * maxHeight * 0.5F;
	y2 = (b.p.y + 1.0F) * maxHeight * 0.5F;
	y3 = (c.p.y + 1.0F) * maxHeight * 0.5F;

	minX = static_cast<int>(roundf(fmaxf(min({ x1, x2, x3 }), 0.0F)));
	maxX = static_cast<int>(roundf(fminf(max({ x1, x2, x3 }), maxWidth)));
	minY = static_cast<int>(roundf(fmaxf(min({ y1, y2, y3 }), 0.0F)));
	maxY = static_cast<int>(roundf(fminf(max({ y1, y2, y3 }), maxHeight)));

	yy, xx;

	//__m128 Xdiff, Ydiff, Xvals, Yvals;
	//__m128 DXs;

	//DXs.m128_f32[0] = dx12;
	//DXs.m128_f32[1] = dx23;
	//DXs.m128_f32[2] = dx31;


	double val;
	for (yy = minY; yy <= maxY; ++yy)
	{
		for (xx = minX; xx <= maxX; ++xx)
		{
			x = (xx / maxWidth  / 0.5) - 1.0;
			y = (yy / maxHeight / 0.5) - 1.0;

			//clock_t begin = std::clock();
			
			//Xdiff.m128_f32[0] = xx - x1;
			//Xdiff.m128_f32[1] = xx - x2;
			//Xdiff.m128_f32[2] = xx - x3;
			//Ydiff.m128_f32[0] = yy - y1;
			//Ydiff.m128_f32[1] = yy - y2;
			//Ydiff.m128_f32[2] = yy - y3;
			//			
			//Xvals = _mm_mul_ps(DXs, Xdiff);
			//Yvals = _mm_mul_ps(DXs, Ydiff);
			
			//val = Yvals.m128_f32[0] - Xvals.m128_f32[0];
			val = dx12 * (yy - y1) - dy12 * (xx - x1);
			if (topLeft1 ? (val < 0) : (val <= 0)) continue;

			//val = Yvals.m128_f32[1] - Xvals.m128_f32[1];
			val = dx23 * (yy - y2) - dy23 * (xx - x2);
			if (topLeft2 ? (val < 0) : (val <= 0)) continue;

			//val = Yvals.m128_f32[2] - Xvals.m128_f32[2];
			val = dx31 * (yy - y3) - dy31 * (xx - x3);
			if (topLeft3 ? (val < 0) : (val <= 0)) continue;

			//time_ps += std::clock() - begin;

			lambda[0] = ((dy23 * (x - c.p.x)) + (dx32 * (y - c.p.y))) * lambda1den;
			lambda[1] = ((dy31 * (x - c.p.x)) + (dx13 * (y - c.p.y))) * lambda2den;
			lambda[2] = 1.0F - lambda[0] - lambda[1];

			depth = (lambda[0] * a.p.z) + (lambda[1] * b.p.z) + (lambda[2] * c.p.z);

			idX = (x + 1) * buffer.w * 0.5f;
			idY = (y + 1) * buffer.h * 0.5f;
			if ((depth < 0) || (depth < buffer.depth[(buffer.w * buffer.h - 1) - (idX + idY * buffer.w)]))
				continue;

			f = lambda;
			f *= 1.0f / (f[0] + f[1] + f[2]);

			pos = a.p2 * lambda[0] + b.p2 * lambda[1] + c.p2 * lambda[2];
			norm = (a.n * f[0] + b.n * f[1] + c.n * f[2]);
			uv = a.uv * f[0] + b.uv * f[1] + c.uv * f[2];

			norm.Normalize();

			GetColor(a, b, c);

			buffer.color[(buffer.w * buffer.h - 1) - (idX + idY * buffer.w)] = Linear::Saturated(finalColor);
			buffer.depth[(buffer.w * buffer.h - 1) - (idX + idY * buffer.w)] = depth;
		}
	}
}

void Rasterizer::GetColor(const Vertex& a, const Vertex& b, const Vertex& c)
{
	static float4 x, y;

	colorMult = a.c * lambda[0] + b.c * lambda[1] + c.c * lambda[2];
	finalColor = { 0,0,0 };

	for (int i = 0; i < lights.size(); i++)
	{
		lightVec = lights[i]->GetDir(pos);
		attenuation = lights[i]->GetIntensity(lightVec);

		x = pos;
		x.Normalize();
		lightVec.Normalize();
		halfVec = (x + lightVec);
		halfVec.Normalize();
		halfVec *= -1;
		
		LdotN = fmax(0.0f, Linear::Dot(lightVec, norm));
		NdotH = fmax(0.0f, -Linear::Dot(halfVec, norm));
		
		x = lights[i]->diffuse;
		x *= attenuation * LdotN;
		Linear::ADD(lights[i]->ambient, x, y);
		Linear::MUL(y, colorMult, x); // x = ambient + diffuse
		y = lights[i]->specular;
		y *= std::pow(NdotH, 5); // y = specular
		Linear::ADD(x, y, tmpCol);

		//tmpCol = lights[i]->ambient;							// ambient
		//tmpCol +=lights[i]->diffuse * (attenuation * LdotN);	// diffuse
		//tmpCol *= colorMult;
		//tmpCol += lights[i]->specular * std::pow(NdotH, 5);		// specular

		finalColor += tmpCol;

	}
}
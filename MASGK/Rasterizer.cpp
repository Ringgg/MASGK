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

	topLeft1 = dy12 < 0 || dx12 < 0;
	topLeft2 = dy23 < 0 || dx23 < 0;
	topLeft3 = dy31 < 0 || dx31 < 0;

	maxWidth = buffer.w - 1.0;
	maxHeight = buffer.h - 1.0;

	x1 = (a.p.x + 1.0) * 0.5 * maxWidth;
	x2 = (b.p.x + 1.0) * 0.5 * maxWidth;
	x3 = (c.p.x + 1.0) * 0.5 * maxWidth;

	y1 = (a.p.y + 1.0) * 0.5 * maxHeight;
	y2 = (b.p.y + 1.0) * 0.5 * maxHeight;
	y3 = (c.p.y + 1.0) * 0.5 * maxHeight;

	minX = max(min({ x1, x2, x3 }), 0.0f);
	maxX = min(max({ x1, x2, x3 }), maxWidth);
	minY = max(min({ y1, y2, y3 }), 0.0f);
	maxY = min(max({ y1, y2, y3 }), maxHeight);

	for (yy = minY; yy <= maxY; ++yy)
	{
		for (xx = minX; xx <= maxX; ++xx)
		{
			if (topLeft1 ? (dx12 * (yy - y1) - dy12 * (xx - x1) < 0) : (dx12 * (yy - y1) - dy12 * (xx - x1) <= 0)) continue;
			if (topLeft2 ? (dx23 * (yy - y2) - dy23 * (xx - x2) < 0) : (dx23 * (yy - y2) - dy23 * (xx - x2) <= 0)) continue;
			if (topLeft3 ? (dx31 * (yy - y3) - dy31 * (xx - x3) < 0) : (dx31 * (yy - y3) - dy31 * (xx - x3) <= 0)) continue;

			x = ((float)xx / maxWidth / 0.5) - 1.0;
			y = ((float)yy / maxHeight / 0.5) - 1.0;

			lambda[0] = ((dy23 * (x - c.p.x)) + (dx32 * (y - c.p.y))) * lambda1den;
			lambda[1] = ((dy31 * (x - c.p.x)) + (dx13 * (y - c.p.y))) * lambda2den;
			lambda[2] = 1.0F - lambda[0] - lambda[1];

			depth = (lambda[0] * a.p.z) + (lambda[1] * b.p.z) + (lambda[2] * c.p.z);

			if ((depth < 0) || (depth < buffer.depth[(buffer.w * buffer.h - 1) - (xx + yy * buffer.w)]))
				continue;

			f = lambda;
			f *= 1.0f / (f[0] + f[1] + f[2]);

			pos = a.p2 * lambda[0] + b.p2 * lambda[1] + c.p2 * lambda[2];
			norm = (a.n * f[0] + b.n * f[1] + c.n * f[2]);
			uv = a.uv * f[0] + b.uv * f[1] + c.uv * f[2];

			norm.Normalize();

			GetColor(a, b, c);

			buffer.color[(buffer.w * buffer.h - 1) - (xx + yy * buffer.w)] = Linear::Saturated(finalColor);
			buffer.depth[(buffer.w * buffer.h - 1) - (xx + yy * buffer.w)] = depth;
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
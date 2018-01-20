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

	dx12 = a.p.x - b.p.x;    dx23 = b.p.x - c.p.x;    dx31 = c.p.x - a.p.x;
	dy12 = a.p.y - b.p.y;    dy23 = b.p.y - c.p.y;    dy31 = c.p.y - a.p.y;
	dx13 = a.p.x - c.p.x;    dx32 = c.p.x - b.p.x;

	lambda1den = 1 / (dy23*dx13 + dx32*(a.p.y - c.p.y));
	lambda2den = 1 / (dy31*dx23 + dx13*(dy23));

	topLeft1 = dy12 < 0 || dy12 == 0 && dx12 < 0;
	topLeft2 = dy23 < 0 || dy23 == 0 && dx23 < 0;
	topLeft3 = dy31 < 0 || dy31 == 0 && dx31 < 0;

	minX = fmax( fmin(a.p.x, fmin(b.p.x, c.p.x) ), -1.0F);
	maxX = fmin( fmax(a.p.x, fmax(b.p.x, c.p.x) ),  1.0F);
	minY = fmax( fmin(a.p.y, fmin(b.p.y, c.p.y) ), -1.0F);
	maxY = fmin( fmax(a.p.y, fmax(b.p.y, c.p.y) ),  1.0F);
	stepX = 1.0 / buffer.w;
	stepY = 1.0 / buffer.h;

	for (y = minY; y < maxY; y += stepY)
	{
		for (x = minX; x < maxX; x += stepX)
		{
			if (topLeft1 ? (dx12 * (y - a.p.y) - dy12 * (x - a.p.x) < 0) : (dx12* (y - a.p.y) - dy12 * (x - a.p.x) < 0)) continue;
			if (topLeft2 ? (dx23 * (y - b.p.y) - dy23 * (x - b.p.x) < 0) : (dx23* (y - b.p.y) - dy23 * (x - b.p.x) < 0)) continue;
			if (topLeft3 ? (dx31 * (y - c.p.y) - dy31 * (x - c.p.x) < 0) : (dx31* (y - c.p.y) - dy31 * (x - c.p.x) < 0)) continue;

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
	finalColor = { 0,0,0 };
	for (int i = 0; i < lights.size(); i++)
	{		
		float4 lightVec = lights[i]->GetDir(pos);
		float attenuation = lights[i]->GetIntensity(lightVec);
		lightVec.Normalize();

		float4 viewVec = -(pos.Normalized());
		float4 halfVec = (viewVec + -lightVec).Normalized();

		float LdotN = fmax(0.0f, Linear::Dot(lightVec, norm));
		float NdotH = fmax(0.0f, -Linear::Dot(halfVec, norm));

		if (LdotN > 0)
			LdotN = LdotN;

		ambient = lights[i]->ambient;
		diffuse = lights[i]->diffuse * attenuation * LdotN;
		specular = (lights[i]->specular * std::pow(NdotH, 5));

		ambient *= a.c * lambda[0] + b.c * lambda[1] + c.c * lambda[2];
		diffuse *= a.c * lambda[0] + b.c * lambda[1] + c.c * lambda[2];

		finalColor = finalColor + ambient + diffuse + specular;
	}
}
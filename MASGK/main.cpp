#include <iostream>
#include <ctime>

#include "Lib3d.h"
#include "Mesh.h"
#include "SimpleTriangle.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Cube.h"
#include "Rasterizer.h"

using namespace std;

int main()
{
	Buffer b(512, 512);
	Rasterizer r(b);

	Light L(float4(4, 1, -4, 0), float4(1, 0, -1, 0));
	L.attenuation = 0.1f;
	L.ambient = { 0.05f, 0.05f, 0.05f };
	L.diffuse = { 1.0f, 1.0f, 1.0f };
	L.specular = { 1.0f, 1.0f, 1.0f };

	Light L2(float4(-4, 2.5, 4, 1), float4(0, 0, 0, 0));
	L2.attenuation = 0.01f;
	L2.diffuse = { 1.0f, 0.0f, 0.0f };
	L2.specular = { 1.0f, 1.0f, 1.0f };


	VertexProcessor vp;

	vp.lights.push_back(&L);
	r.lights.push_back(&L);
	vp.lights.push_back(&L2);
	r.lights.push_back(&L2);

	vp.SetPerspective(60, 1, 0.2, 100);
	vp.SetLookAt(float3(4, -2, 4), float3(0, 0, 0), float3(0, 1, 0));

	clock_t time = 0;
	clock_t begin, end;

	for (int q = 0; q < 11; ++q)
	{
		if (q > 0)
			begin = std::clock();

		vp.SetIdentity();
		vp.MultByScale(float3(2.5, 1.5, 2.5));
		vp.MultByTrans(float3(0, -1, 0));

		for (int i = 0; i < 10; ++i)
		{
			vp.MultByScale(float3(0.85, 0.85, 0.85));
			vp.MultByTrans(float3(0, 0.4, 0));
			Cylinder(30).Draw(r, vp);
		}

		if (q > 0)
		{
			end = std::clock();
			time += end - begin;
		}
	}
	
	float percent = (r.time_ps / 10000.0) / 0.1;
	std::cout << "time: " << time / 10000.0f << "\n";// << "s, of all: " << percent << "\n";

	b.writeTGA("blabla.tga");

	string s;
	cin >> s;

    return 0;
}

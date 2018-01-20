#include <iostream>

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

	Light L(float4(0, 1, -4, 0), float4(1, 0, -1, 0));
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
	vp.SetIdentity();

	vp.MultByScale(float3(2.5,1.5,2.5));
	vp.MultByTrans(float3(0, -1, 0));
	
	for (int i = 0; i < 10; ++i)
	{
		vp.MultByScale(float3(0.85, 0.85, 0.85));
		vp.MultByTrans(float3(0, 0.4, 0));
		Cylinder(20).Draw(r, vp);
	}
	//vp.MultByTrans(float3(0, 0.15, 0));
	//Cone(20).Draw(r, vp);
	//
	//vp.SetIdentity();
	//vp.MultByScale(float3(5, 0.5f, 5));
	//vp.MultByTrans(float3(0, -1, 0));
	//vp.MultByRot(30, float3(0, 1, 0));
	//Cube().Draw(r, vp);

	b.writeTGA("blabla.tga");

    return 0;
}
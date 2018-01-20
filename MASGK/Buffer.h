#pragma once

struct Buffer;

#include<stdint.h>
#include<string>
#include<cstdio>
#include"float3.h"
#include"Lib3d.h"

using namespace std;

struct Buffer
{
    float3* color;
    float* depth;
    u16 w, h;

    Buffer(int w, int h);
    virtual ~Buffer();

    void setSize();
    void clearColor();
    void clearDepth();
    s32 writeTGA(string path);
};

#include "Buffer.h"

Buffer::Buffer(int w, int h) : w(w), h(h)
{
	color = new float3[w * h];
	depth = new float[w * h];

	clearColor();
	clearDepth();
}

Buffer::~Buffer() { }
void Buffer::setSize() { }

void Buffer::clearColor() 
{
	float3 black(0, 0, 0);
	float3 white(1, 1, 1);

	for (int i = 0; i < w*h; ++i)
		color[i] = black;
}

void Buffer::clearDepth()
{
	for (int i = 0; i < w*h; ++i)
		depth[i] = 0;
}

s32 Buffer::writeTGA(const string path)
{
	FILE* f;;
	fopen_s(&f, path.c_str(), "wb+");

	if (NULL == f) return -1;

	putc(0, f);
	putc(0, f);
	putc(2, f);                     /* uncompressed RGB */
	putc(0, f); putc(0, f);
	putc(0, f); putc(0, f);
	putc(0, f);			
	putc(0, f); putc(0, f);         /* X origin */
	putc(0, f); putc(0, f);         /* y origin */
	putc((w & 0x00FF), f);
	putc((w & 0xFF00) / 256, f);
	putc((h & 0x00FF), f);
	putc((h & 0xFF00) / 256, f);
	putc(24, f);                    /* 24 bit bitmap */
	putc(0, f);

	for (int i = 0; i < w*h; ++i)
	{
		putc((u8)(color[i].b * 255), f);
		putc((u8)(color[i].g * 255), f);
		putc((u8)(color[i].r * 255), f);
	}

    return 0;
}

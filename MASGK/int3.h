#pragma once

struct int3;

struct int3
{
	int a, b, c;

	int3(): a(0), b(0), c(0) {}
	int3(int a, int b, int c): a(a), b(b), c(c) {}
	
	int& operator[](const int i) const { return *((int*)this + i); }
};

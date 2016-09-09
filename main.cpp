#include <cstdio>
#include "buffer.h"

int main()
{
	auto b = kit::Buffer();
	b.init(32);
	b << (double)23.0 << (double)12 << "sdf";
	char c[12] = "sdfds";
	b.writeBuffer(c, size_t(2));
	printf("buffer size:%d\n", (int)b.getWritableSize());

	double a, d;
	char ss[342] = {0};
	char* p = (char*)ss;
	b >> a >> d >> p; 
	printf("read::  %lf,%lf,%s\n", a, d, p);

	//scanf("%s", ss);
	return 0;
}

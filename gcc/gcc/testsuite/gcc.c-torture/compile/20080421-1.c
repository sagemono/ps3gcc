typedef unsigned long long uint64_t;
typedef __SIZE_TYPE__ uintptr_t;

uint64_t data;
float x;

void f()
{
    float *fp = (float *)((uintptr_t)&(data));
    x = *fp;
}

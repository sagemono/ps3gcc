/* { dg-do compile } */
/* { dg-options "-O2 -w -fdump-tree-final_cleanup" } */

#define vector __attribute__((vector_size(16) ))


typedef struct {
	vector float a, a1, a2;
	vector float b, b1, b2;
} VectorFloat2;

VectorFloat2 test(vector float a)
{
	VectorFloat2 data;
	data.a = a + a;
	data.b = a + a;
	return data;
}

int main(void)
{
	VectorFloat2 a;
	volatile VectorFloat2 b;
	volatile VectorFloat2 c;
	a = test(a.a);
	b = test(a.a);
	c = test(a.b);

	// use 'c'
	volatile float *f = (volatile float *)&c;
	printf("%f %f %f %f\n", f[0], f[1], f[2], f[3]);
	printf("%f %f %f %f\n", f[4], f[5], f[6], f[7]);

	return 0;
}

/* { dg-final { scan-tree-dump-times "return slot optimization" 1 "final_cleanup"} } */

/* { dg-final { cleanup-tree-dump "final_cleanup" } } */


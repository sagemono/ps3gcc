/* { dg-do compile } */
/* { dg-options "-Werror=-WnonPOD-varargs" } */
/* Test to check that error is not emitted here as we don't pass a nonPOD
 * in a variable arguments function */
class A
{
 private:
	int x;
	friend void callbig (A* a);
 public:
	void big (int p, ...)
	{
		x = x + p;
	}
};

void callbig ( A *a)
{
 a->big(a->x); /* { dg-bogus "" } */
}

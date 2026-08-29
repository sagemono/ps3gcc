/* { dg-options "" } */
/* { dg-do compile } */

/* We don't want a warning for volatile reference access when it
   comes from a function call. */

struct vclass
{
  volatile vclass &operator=(volatile vclass &other) volatile
  {
    i = other.i;
    return *this;
  }
  int i;
};

volatile vclass f;
volatile vclass f1;

volatile vclass & g();

void h(void)
{
 f = f1; // { dg-bogus "will not be accessed in statement" }
 g(); // { dg-bogus "will not be accessed in statement" }
}

// { dg-do compile }
// { dg-options "-O2 -w" }

// We used to crash here as TYPE_SIZE was not set for the array type.

extern int s_instance[];

struct sysm_proxy
{
  int a;
};

static struct sysm_proxy* the_instance() {
  return (struct sysm_proxy*)(s_instance);
 }

int use(void)
{
  return the_instance()->a;
}

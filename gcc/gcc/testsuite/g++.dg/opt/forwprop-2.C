// { dg-do compile }
// { dg-options "-O2 -w" }

// We used to crash here as we would convert the access in g to be a VCE but
// it is from a function declaration which we should not do.


class ios_base {   };
struct _Smanip  {
  void (*_Pfun)(ios_base&, int);
  int _Manarg;
};
inline  ios_base&
operator<<(   ios_base& _Ostr, const _Smanip& _Manip)
{
  (*_Manip._Pfun)(_Ostr, _Manip._Manarg);
}
_Smanip setiosflags(int);
_Smanip setw(int);

extern ios_base debug_inf;
void f(int);
int g(void)   {
  unsigned int crt_ignore_begin = *(unsigned int *)setiosflags;
  f(crt_ignore_begin);
}
void h(void) {
  g();
  debug_inf << setiosflags(1) << setw(3) << setw(3);
}


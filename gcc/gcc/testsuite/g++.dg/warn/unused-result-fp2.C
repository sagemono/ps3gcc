// PR c++/27371
class T {
public:
  T();
  T(const T &);
};

// functions
T func(void);
T funcw(void) __attribute__((warn_unused_result));

// function pointers
T (*pnn)(void);
T (*pnw)(void);
T (*pwn)(void) __attribute__((warn_unused_result));
T (*pww)(void) __attribute__((warn_unused_result));

// virtual functions
class B {
public:
  virtual T fnn(void);
  virtual T fnw(void);
  virtual T fwn(void) __attribute__((warn_unused_result));
  virtual T fww(void) __attribute__((warn_unused_result));
};

class D : public B {
public:
  virtual T fnn(void);
  virtual T fnw(void) __attribute__((warn_unused_result));
  virtual T fwn(void);
  virtual T fww(void) __attribute__((warn_unused_result));
};

int main(void)
{
  // function calls
  func();
  funcw(); // { dg-warning "ignoring" }
  T a = func();
  T b = funcw();

  // function pointers
  pnn = func;
  pnw = funcw;
  pwn = func;
  pww = funcw;
  (*pnn)();
  (*pnw)();
  (*pwn)(); // { dg-warning "ignoring" }
  (*pww)(); // { dg-warning "ignoring" }
  T c = (*pnn)(); 
  T d = (*pnw)(); 
  T e = (*pwn)(); 
  T f = (*pww)(); 

  // virtual functions
  B* pB = new D();
  pB->fnn();
  pB->fnw();
  pB->fwn(); // { dg-warning "ignoring" }
  pB->fww(); // { dg-warning "ignoring" }
  T g = pB->fnn();
  T h = pB->fnw();
  T i = pB->fwn();
  T j = pB->fww();

  return 0;
}

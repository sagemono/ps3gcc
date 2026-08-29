// PR c++/38172

struct MyClass
{
};

typedef struct MyClass MyClass;

int Bar1( void ) __attribute__((warn_unused_result)) __attribute__((noinline));
MyClass Bar2( void ) __attribute__((warn_unused_result)) __attribute__((noinline));

int Bar1( void )
{
    return 0;
}

MyClass Bar2( void )
{
}

void Foo( void )
{
    Bar1(); // { dg-warning "ignoring" }
    Bar2(); // { dg-warning "ignoring" }
}

void Foo2( void )
{
    int a = Bar1();
    MyClass m = Bar2();
}

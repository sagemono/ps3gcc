// PR c++/46643

template<typename> struct S { };

template<typename T>
class foo {
public:
    S<T> bar() __attribute__((warn_unused_result));
    int baz() __attribute__((warn_unused_result));
};

template<typename T> inline
S<T> foo<T>::bar() { return S<T>(); }

template<typename T> inline
int foo<T>::baz() { return 0; }

int main(void) {
    foo<int> b;
    b.bar(); // { dg-warning "ignoring" }
    b.baz(); // { dg-warning "ignoring" }
    return 0;
}

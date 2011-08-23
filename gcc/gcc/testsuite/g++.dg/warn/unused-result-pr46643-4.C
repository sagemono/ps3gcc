// PR c++/46643

struct S { S() { } };

template<typename T>
class foo {
public:
    S bar() __attribute__((warn_unused_result));
};

template<typename T> inline
S foo<T>::bar() { return S(); }

int main(void) {
    foo<int> b;
    b.bar(); // { dg-warning "ignoring" }
    return 0;
}

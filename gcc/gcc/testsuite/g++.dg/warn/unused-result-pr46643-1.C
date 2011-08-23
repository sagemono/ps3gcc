// PR c++/46643

#include <map>

template<typename T>
class foo {
public:
    std::pair<T, int> bar() __attribute__((warn_unused_result));
    int baz() __attribute__((warn_unused_result));
};

template<typename T> inline
std::pair<T, int> foo<T>::bar() { return std::pair<T, int>(0, 0); }

template<typename T> inline
int foo<T>::baz() { return 0; }

int main(void) {
    foo<int> b;
    b.bar(); // { dg-warning "ignoring" }
    b.baz(); // { dg-warning "ignoring" }
    return 0;
}

// PR c++/46643

class foo {
public:
    int baz() __attribute__((warn_unused_result));
private:
    int a;
};

foo g(void) __attribute__((warn_unused_result));

int main(void) {
    foo b;
    g(); // { dg-warning "ignoring" }
    return 0;
}

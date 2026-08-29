// PR c++/31063
// Illustrates some bugs related to __attribute__((warn_unused_result)).

struct Vector {
    float x, y, z;
    inline Vector() {}
    inline Vector(float _x,float _y,float _z) : x(_x), y(_y), z(_z) {}

    // BUG: Toggling the presence of a copy constructor (doesn't matter which
    // of the ones below you use) will affect the generation of the warnings.
//  inline Vector(const Vector &a) { x = a.x; y = a.y; z = a.z; }
    inline Vector(const Vector &a) : x(a.x), y(a.y), z(a.z) {}

    inline Vector operator-(const Vector &a) const __attribute__((warn_unused_result)) {
        return Vector(x - a.x, y - a.y, z - a.z );
    }
};

struct Box {
    Vector min, max;

    // BUG: generates spurrious warning about not using result from operator-.
    // If you remove the copy constructor, the warning goes away
    inline Vector size() const { return max - min; }
};

void foo() {
    Vector a, b;

    // BUG: this *should* generate a warning, but you will only get a warning
    // if you have the copy constructor.  If you remove the copy constructor,
    // the warning goes away
    a-b; // { dg-warning "ignoring" }
}

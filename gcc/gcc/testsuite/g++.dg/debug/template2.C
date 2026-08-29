// { dg-do compile }
// { dg-skip-if "" { *-*-* } "-gdwarf-21" "" }

typedef int DefaultAllocatorTrait;
typedef int SmallArrayData;
template<typename T, typename DataHolderClass, typename
AllocatorTrait = DefaultAllocatorTrait>
struct BaseArray
{
    typedef T ValueType;
    typedef BaseArray<T, DataHolderClass> Self;
    typedef T * Iterator;
    typedef const T * ConstIterator;
    int emitmeplease;
BaseArray(const Self & other);
BaseArray( const char* tag = 0);
};

template < typename T, typename AllocatorTrait = DefaultAllocatorTrait >
struct SmallArray : public BaseArray < T, SmallArrayData,
AllocatorTrait >::Self
{
    typedef BaseArray< T, SmallArrayData, AllocatorTrait > Base;
    inline explicit SmallArray( const char* tag = 0 )
        : Base( tag )
    {}
};

template < typename T, typename AllocatorTrait = DefaultAllocatorTrait >
struct Array : public SmallArray < T, AllocatorTrait >
{
    typedef SmallArray< T, AllocatorTrait > Base;
    inline explicit Array( const char* tag = 0 )
        : Base( tag )
    {}
};
Array<int> b;

// We should make sure emitmeplease is emitted in the generated assembly.
// { dg-final { scan-assembler "emitmeplease" } }


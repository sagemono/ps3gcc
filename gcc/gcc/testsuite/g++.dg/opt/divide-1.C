// { dg-do run }
// { dg-options "-Os -fno-exceptions" }
// This test used to fail under PPU LV2 as the divide was using a multiple which was
// being combined with a word compare, the mullw. instruction does a full 32 by 32 multiply
// resulting in a full 64bit result.  And since the recording part of the instruction compares
// against 64bit, we end up with the wrong result as there was an overflow.

#include <vector>
#include <stdlib.h>

struct sItem
{
    unsigned	i1,i2,i3;
};

int main( int argc, char* argv[] )
{
    std::vector<sItem> vec;
    vec.resize(1);

    if( (int)vec.size() > 0 )
        exit (0);
    else
        abort ();
    return 0;
}

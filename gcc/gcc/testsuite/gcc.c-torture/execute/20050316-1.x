# This test assumes certain overlap of data layout among different data types.
# Unfortunately, on SPU it is the high half of long long to overlap with a 
# narrower integer type.
set torture_execute_xfail "spu-*-*"

return 0


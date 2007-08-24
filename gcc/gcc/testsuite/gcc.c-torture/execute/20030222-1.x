# using inline assembly to convert long long to int is not working quite right
# on the spu. An extra shift-left-4-byte is needed.
set torture_execute_xfail "spu-*-*"

return 0


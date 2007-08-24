# CELL LOCAL Begin
# This test fails because SPU rounds single precision floating point
# numbers towards zero.
if { [istarget "spu-*"] } {
	set torture_execute_xfail "spu-*"
}
return 0
# CELL LOCAL End

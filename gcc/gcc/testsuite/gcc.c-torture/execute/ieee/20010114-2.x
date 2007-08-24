# CELL LOCAL Begin
# This doesn't work on CELL SPU because single precision floats are
# always rounded toward 0.

if { [istarget "spu-*-*"] } {
	set torture_execute_xfail "spu-*-*"
}
return 0
# CELL LOCAL End

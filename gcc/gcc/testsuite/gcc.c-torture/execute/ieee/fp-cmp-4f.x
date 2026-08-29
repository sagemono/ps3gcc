# CELL LOCAL Begin
# SPU does not support NaN in single precision.
if { [istarget "spu-*"] } {
	set torture_execute_xfail "spu-*"
}
return 0
# CELL LOCAL End

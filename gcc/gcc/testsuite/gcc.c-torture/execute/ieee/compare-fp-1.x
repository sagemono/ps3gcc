# CELL LOCAL Begin
# SPU does not support NaN & Inf in single precision.
if { [istarget "spu-*"] } {
	set torture_execute_xfail "spu-*"
}
return 0
# CELL LOCAL End

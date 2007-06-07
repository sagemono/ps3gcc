lappend additional_flags "-fno-trapping-math"
# CELL LOCAL Begin
# SPU does not support NaN & Inf in single precision.
if { [istarget "spu-*"] } {
	set torture_execute_xfail "spu-*"
}
# CELL LOCAL End
return 0

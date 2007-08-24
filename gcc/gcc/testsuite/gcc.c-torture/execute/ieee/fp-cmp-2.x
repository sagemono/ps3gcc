# CELL LOCAL Begin
# This doesn't work on CELL SPU because it does not support infinities 
# and NaN in single precision floats.

# This test is not even compilable because nanf cannot be constant folded.
if { [istarget "spu-*-*"] } {
	return 1
}

return 0
# CELL LOCAL End

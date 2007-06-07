# CELL LOCAL Begin
# SPU fails this test because it always round single precision floating
# point numbers towards zero.
if { [istarget "spu-*"] } {
	set torture_execute_xfail "spu-*"
}

return 0
# CELL LOCAL End

# CELL LOCAL Begin
# This doesn't work on CELL testing since we specify -testing and DBL_MAX
# ends up being _Dbl._Dmax._Double, which is not a constant.

if { [istarget "spu-*-lv2"] || [istarget "ppu-*-lv2"] } {
	# not compilable
	return 1
}

return 0
# CELL LOCAL End

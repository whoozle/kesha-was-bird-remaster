	.module init
	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL

	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP
	.area   _CODE
	.area   _GSINIT

_gsinit::
	di
	ld	bc, #l__INITIALIZER
	ld	a, b
	or	a, c
	jr	Z, gsinit_next$
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir
gsinit_next$:

	.area   _GSFINAL
	ei
	ret

_halt::
	halt
	ret

_get_r::
	ld a, r
	ld l, a
	ret

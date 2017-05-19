
@{{BLOCK(bg)

@=======================================================================
@
@	bg, 16x16@8, 
@	Transparent color : FF,00,FF
@	+ palette 23 entries, not compressed
@	Total size: 46 = 46
@
@	Time-stamp: 2017-05-19, 15:16:38
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.14
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bgPal		@ 48 unsigned chars
	.hidden bgPal
bgPal:
	.hword 0x7C1F,0x1084,0x5A8C,0x0D32,0x2E3A,0x3528,0x6B53,0x1DB6
	.hword 0x7BBD,0x4E09,0x5AD3,0x41EC,0x1574,0x6B38,0x3DEF,0x5E50
	.hword 0x6ED5,0x49AA,0x21D6,0x41EF,0x22AF,0x3754,0x2AF1

@}}BLOCK(bg)

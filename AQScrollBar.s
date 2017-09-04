
; Maxon C++ Compiler
; PROGDIR:projects/AQ/AQScrollBar.cpp


	XREF	rastPort__AQWidget__T
	XREF	mouseReleaseEvent__AQWidget__TRC12IntuiMessage
	XREF	mouseMoveEvent__AQWidget__TRC12IntuiMessage
	XREF	mousePressEvent__AQWidget__TRC12IntuiMessage
	XREF	paintEvent__AQWidget__TRC06AQRect
	XREF	keyEvent__AQWidget__TRC12IntuiMessage
	XREF	event__AQWidget__TR12IntuiMessage
	XREF	update__AQWidget__T
	XREF	setExpanding__AQWidget__Tcc
	XREF	setPreferredSize__AQWidget__TRC07AQPoint
	XREF	setMaximumSize__AQWidget__TRC07AQPoint
	XREF	setMinimumSize__AQWidget__TRC07AQPoint
	XREF	size__AQWidget__CT
	XREF	_0dt__AQWidget__T
	XREF	_0ct__AQWidget__TP08AQWidget
	XREF	_GfxBase

	SECTION ":0",CODE


	XDEF	_0ct__AQScrollBar__TcP08AQWidget
_0ct__AQScrollBar__TcP08AQWidget
L14	EQU	-$3C
	link	a5,#L14+12
L15	EQU	$4804
	movem.l	d2/a3/a6,-(a7)
	move.b	$C(a5),d2
	move.l	$8(a5),a3
	move.l	$E(a5),-(a7)
	move.l	a3,a6
	pea	(a6)
	jsr	_0ct__AQWidget__TP08AQWidget
	addq.l	#$8,a7
	move.l	$8(a5),a3
	move.l	a5,a6
	move.l	a3,-(a6)
	move.l	#_0dt__AQWidget__T,-(a6)
	XREF	sym_handlers
	move.l	sym_handlers,-(a6)
	move.l	a6,sym_handlers
	move.l	$8(a5),a3
	move.b	d2,$76(a3)
	clr.l	$78(a3)
	move.l	#$A,$7C(a3)
	clr.l	$80(a3)
	move.l	#$5A,$84(a3)
	tst.b	d2
	beq.b	L6
	clr.b	-(a7)
	move.b	#1,-(a7)
	move.l	$8(a5),-(a7)
	jsr	setExpanding__AQWidget__Tcc
	addq.l	#$8,a7
	bra	L7
L6
	move.b	#1,-(a7)
	clr.b	-(a7)
	move.l	$8(a5),-(a7)
	jsr	setExpanding__AQWidget__Tcc
	addq.l	#$8,a7
	lea	-$14(a5),a3
	move.l	#$1E,-$18(a5)
	move.l	#$9,-$1C(a5)
	move.l	a3,a6
	move.l	a6,-$20(a5)
	move.l	-$1C(a5),(a6)
	move.l	-$18(a5),4(a6)
	pea	-$14(a5)
	move.l	$8(a5),-(a7)
	jsr	setMinimumSize__AQWidget__TRC07AQPoint
	addq.l	#$8,a7
	lea	-$28(a5),a3
	move.l	#$3E8,-$18(a5)
	move.l	#$9,-$1C(a5)
	move.l	a3,a6
	move.l	a6,-$20(a5)
	move.l	-$1C(a5),(a6)
	move.l	-$18(a5),4(a6)
	pea	-$28(a5)
	move.l	$8(a5),-(a7)
	jsr	setMaximumSize__AQWidget__TRC07AQPoint
	addq.l	#$8,a7
	lea	-$30(a5),a3
	move.l	#$1E,-$18(a5)
	move.l	#$9,-$1C(a5)
	move.l	a3,a6
	move.l	a6,-$20(a5)
	move.l	-$1C(a5),(a6)
	move.l	-$18(a5),4(a6)
	pea	-$30(a5)
	move.l	$8(a5),-(a7)
	jsr	setPreferredSize__AQWidget__TRC07AQPoint
	addq.l	#$8,a7
L7
	move.l	-$C(a5),sym_handlers
	movem.l	(a7)+,d2/a3/a6
	unlk	a5
	rts

	XDEF	_0dt__AQScrollBar__T
_0dt__AQScrollBar__T
L16	EQU	4
L17	EQU	$800
	move.l	a3,-(a7)
L18
	move.l	L16+4(a7),a3
	pea	(a3)
	jsr	_0dt__AQWidget__T
	addq.l	#4,a7
	move.l	(a7)+,a3
	rts

	XDEF	value__AQScrollBar__CT
value__AQScrollBar__CT
L19	EQU	0
L20	EQU	0
	move.l	L19+4(a7),a0
	move.l	$78(a0),d0
	rts

	XDEF	setMaximum__AQScrollBar__Ti
setMaximum__AQScrollBar__Ti
L21	EQU	$8
L22	EQU	$804
	movem.l	d2/a3,-(a7)
	move.l	L21+$8(a7),d2
	move.l	L21+4(a7),a3
	cmp.l	$7C(a3),d2
	bne.b	L10
	movem.l	(a7)+,d2/a3
	rts
L10
	move.l	L21+4(a7),a3
	move.l	d2,$7C(a3)
	move.l	L21+4(a7),-(a7)
	jsr	update__AQWidget__T
	addq.l	#4,a7
	movem.l	(a7)+,d2/a3
	rts

	XDEF	setMaximum__AQScrollBar__Ti
setMaximum__AQScrollBar__Ti
L23	EQU	$C
L24	EQU	$80C
	movem.l	d2/d3/a3,-(a7)
	move.l	L23+$8(a7),d3
	move.l	L23+4(a7),a3
	cmp.l	$7C(a3),d3
	bne.b	L10
	movem.l	(a7)+,d2/d3/a3
	rts
L10
	move.l	L23+4(a7),a3
	move.l	d3,$7C(a3)
	move.l	L23+4(a7),-(a7)
	jsr	update__AQWidget__T
	addq.l	#4,a7
	movem.l	(a7)+,d2/d3/a3
	rts

	XDEF	paintEvent__AQScrollBar__TRC06AQRect
paintEvent__AQScrollBar__TRC06AQRect
L25	EQU	-$54
	link	a5,#L25+36
L26	EQU	$4CFC
	movem.l	d2-d7/a2/a3/a6,-(a7)
	move.l	$C(a5),-(a7)
	move.l	$8(a5),a3
	pea	(a3)
	jsr	paintEvent__AQWidget__TRC06AQRect
	addq.l	#$8,a7
	move.l	$8(a5),-(a7)
	jsr	rastPort__AQWidget__T
	addq.l	#4,a7
	move.l	d0,a2
	lea	-$30(a5),a3
	move.l	$8(a5),-(a7)
	pea	(a3)
	jsr	size__AQWidget__CT
	addq.l	#$8,a7
	lea	-$30(a5),a3
	move.l	a3,-$24(a5)
	lea	-$C(a5),a3
	move.l	a3,-$28(a5)
	move.l	-$24(a5),a6
	move.l	(a6),(a3)
	move.l	4(a6),4(a3)
	move.l	$8(a5),a3
	tst.b	$76(a3)
	beq.b	L11
	bra	L13
L11
	move.l	#$8,-$10(a5)
	move.l	-$8(a5),d7
	subq.l	#1,d7
	move.l	d7,d4
	move.l	-$C(a5),d7
	subq.l	#1,d7
	move.l	d7,-$18(a5)
	moveq	#1,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$156(a6)
	move.l	d4,d1
	sub.l	-$10(a5),d1
	addq.l	#2,d1
	moveq	#0,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F0(a6)
	move.l	d4,d1
	subq.l	#2,d1
	move.l	-$C(a5),d0
	bpl.b	L27
	addq.l	#1,d0
L27
	asr.l	#1,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	d4,d1
	sub.l	-$10(a5),d1
	addq.l	#2,d1
	move.l	-$18(a5),d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	-$10(a5),d7
	sub.l	d7,d4
	move.l	d4,d1
	subq.l	#2,d1
	moveq	#0,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F0(a6)
	move.l	d4,d1
	sub.l	-$10(a5),d1
	addq.l	#2,d1
	move.l	-$C(a5),d0
	bpl.b	L28
	addq.l	#1,d0
L28
	asr.l	#1,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	d4,d1
	subq.l	#2,d1
	move.l	-$18(a5),d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	-$10(a5),d7
	sub.l	d7,d4
	moveq	#1,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$156(a6)
	move.l	d4,d3
	move.l	-$18(a5),d2
	moveq	#0,d1
	moveq	#0,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$132(a6)
	moveq	#2,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$156(a6)
	moveq	#0,d1
	move.l	-$18(a5),d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F0(a6)
	move.l	d4,d1
	move.l	-$18(a5),d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	d4,d1
	moveq	#0,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$F6(a6)
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	$78(a3),d0
	sub.l	$80(a6),d0
	move.l	-$8(a5),d7
	subq.l	#1,d7
	move.l	d7,d1
	XREF	intmult
	jsr	intmult
	moveq	#1,d7
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	$84(a3),d6
	sub.l	$80(a6),d6
	move.l	$8(a5),a3
	add.l	$7C(a3),d6
	move.l	d0,d5
	move.l	d7,d0
	move.l	d6,d1
	XREF	lib_div_int
	jsr	lib_div_int
	add.l	d0,d5
	move.l	d5,-$1C(a5)
	move.l	$8(a5),a3
	move.l	$7C(a3),d0
	move.l	-$8(a5),d7
	subq.l	#1,d7
	move.l	d7,d1
	jsr	intmult
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	$84(a3),d7
	sub.l	$80(a6),d7
	move.l	$8(a5),a3
	add.l	$7C(a3),d7
	move.l	d7,d1
	jsr	lib_div_int
	move.l	d0,-$20(a5)
	cmp.l	#2,-$20(a5)
	bhs.b	L12
	move.l	#2,-$20(a5)
L12
	moveq	#0,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$156(a6)
	move.l	-$1C(a5),d3
	add.l	-$20(a5),d3
	move.l	-$18(a5),d2
	subq.l	#1,d2
	move.l	-$1C(a5),d1
	moveq	#1,d0
	move.l	a2,a1
	move.l	_GfxBase,a6
	jsr	-$132(a6)
L13
	movem.l	(a7)+,d2-d7/a2/a3/a6
	unlk	a5
	rts

	XDEF	mousePressEvent__AQScrollBar__TRC12IntuiMessage
mousePressEvent__AQScrollBar__TRC12IntuiMessage
L29	EQU	0
L30	EQU	0
	moveq	#1,d0
	rts

	XDEF	mouseMoveEvent__AQScrollBar__TRC12IntuiMessage
mouseMoveEvent__AQScrollBar__TRC12IntuiMessage
L31	EQU	0
L32	EQU	0
	moveq	#1,d0
	rts

	XDEF	mouseReleaseEvent__AQScrollBar__TRC12IntuiMessage
mouseReleaseEvent__AQScrollBar__TRC12IntuiMessage
L33	EQU	0
L34	EQU	0
	moveq	#1,d0
	rts

	END

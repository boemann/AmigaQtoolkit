
; Maxon C++ Compiler
; PROGDIR:projects/AQ/AQLayout.cpp


	XREF	mouseReleaseEvent__AQWidget__TRC12IntuiMessage
	XREF	mouseMoveEvent__AQWidget__TRC12IntuiMessage
	XREF	mousePressEvent__AQWidget__TRC12IntuiMessage
	XREF	paintEvent__AQWidget__TP08RastPort06tPoint
	XREF	keyEvent__AQWidget__Ti
	XREF	likeToExpandX__AQWidget__CT
	XREF	preferredSize__AQWidget__CT
	XREF	maximumSize__AQWidget__CT
	XREF	minimumSize__AQWidget__CT
	XREF	setPos__AQWidget__TRC06tPoint
	XREF	setSize__AQWidget__TRC06tPoint
	XREF	_0dt__AQWidget__T
	XREF	op__new__Ui

	SECTION ":0",CODE


	XDEF	_0ct__AQLayout__Tic
_0ct__AQLayout__Tic
L49	EQU	$C
L50	EQU	$4880
	movem.l	d7/a3/a6,-(a7)
	move.l	L49+4(a7),a3
	move.l	L49+$8(a7),d0
	asl.l	#2,d0
	move.l	d0,-(a7)
	jsr	op__new__Ui
	addq.l	#4,a7
	move.l	d0,a6
	moveq	#$28,d7
L51
	move.b	(a6)+,(a3)+
	subq.l	#1,d7
	bne.b	L51
	move.l	L49+4(a7),a3
	clr.l	$28(a3)
	move.l	L49+4(a7),a3
	move.b	L49+$C(a7),$2C(a3)
	movem.l	(a7)+,d7/a3/a6
	rts

	XDEF	_0dt__AQLayout__T
_0dt__AQLayout__T
L52	EQU	0
L53	EQU	0
	rts

	XDEF	addWidget__AQLayout__TP08AQWidget
addWidget__AQLayout__TP08AQWidget
L54	EQU	4
L55	EQU	$4000
	move.l	a6,-(a7)
	move.l	L54+4(a7),a0
	move.l	L54+4(a7),a6
	move.l	$28(a6),d0
	asl.l	#2,d0
	add.l	d0,a0
	move.l	L54+$8(a7),(a0)
	move.l	L54+4(a7),a0
	addq.l	#1,$28(a0)
	move.l	(a7)+,a6
	rts

	XDEF	minimumSize__AQLayout__CT
minimumSize__AQLayout__CT
L56	EQU	-$20
	link	a5,#L56+20
L57	EQU	$4C84
	movem.l	d2/d7/a2/a3/a6,-(a7)
	moveq	#0,d2
	bra.b	L3
L2
	lea	-$C(a5),a3
	move.l	$C(a5),a6
	move.l	d2,d7
	asl.l	#2,d7
	add.l	d7,a6
	move.l	(a6),-(a7)
	pea	(a3)
	jsr	minimumSize__AQWidget__CT
	addq.l	#$8,a7
	move.l	$C(a5),a3
	tst.b	$2C(a3)
	beq.b	L6
	move.w	-$C(a5),d7
	add.w	d7,-4(a5)
	move.w	-$A(a5),d7
	cmp.w	-2(a5),d7
	ble.b	L4
	move.w	-$A(a5),-2(a5)
L5
	bra.b	L4
L6
	move.w	-$A(a5),d7
	add.w	d7,-2(a5)
	move.w	-$C(a5),d7
	cmp.w	-4(a5),d7
	ble.b	L4
	move.w	-$C(a5),-4(a5)
L7
L8
L4
	addq.l	#1,d2
L3
	move.l	$C(a5),a3
	cmp.l	$28(a3),d2
	blt.b	L2
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	a6,a2
	lea	-4(a5),a1
	move.l	(a1)+,(a2)+
	movem.l	(a7)+,d2/d7/a2/a3/a6
	unlk	a5
	rts

	XDEF	maximumSize__AQLayout__CT
maximumSize__AQLayout__CT
L58	EQU	-$20
	link	a5,#L58+20
L59	EQU	$4C84
	movem.l	d2/d7/a2/a3/a6,-(a7)
	move.l	$C(a5),a3
	tst.b	$2C(a3)
	beq.b	L9
	move.w	#$7D0,-2(a5)
	bra.b	L10
L9
	move.w	#$7D0,-4(a5)
L10
	moveq	#0,d2
	bra.b	L12
L11
	lea	-$C(a5),a3
	move.l	$C(a5),a6
	move.l	d2,d7
	asl.l	#2,d7
	add.l	d7,a6
	move.l	(a6),-(a7)
	pea	(a3)
	jsr	maximumSize__AQWidget__CT
	addq.l	#$8,a7
	move.l	$C(a5),a3
	tst.b	$2C(a3)
	beq.b	L15
	move.w	-$C(a5),d7
	add.w	d7,-4(a5)
	move.w	-$A(a5),d7
	cmp.w	-2(a5),d7
	bge.b	L13
	move.w	-$A(a5),-2(a5)
L14
	bra.b	L13
L15
	move.w	-$A(a5),d7
	add.w	d7,-2(a5)
	move.w	-$C(a5),d7
	cmp.w	-4(a5),d7
	bge.b	L13
	move.w	-$C(a5),-4(a5)
L16
L17
L13
	addq.l	#1,d2
L12
	move.l	$C(a5),a3
	cmp.l	$28(a3),d2
	blt.b	L11
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	a6,a2
	lea	-4(a5),a1
	move.l	(a1)+,(a2)+
	movem.l	(a7)+,d2/d7/a2/a3/a6
	unlk	a5
	rts

	XDEF	preferredSize__AQLayout__CT
preferredSize__AQLayout__CT
L60	EQU	-$20
	link	a5,#L60+20
L61	EQU	$4C84
	movem.l	d2/d7/a2/a3/a6,-(a7)
	moveq	#0,d2
	bra.b	L19
L18
	lea	-$C(a5),a3
	move.l	$C(a5),a6
	move.l	d2,d7
	asl.l	#2,d7
	add.l	d7,a6
	move.l	(a6),-(a7)
	pea	(a3)
	jsr	preferredSize__AQWidget__CT
	addq.l	#$8,a7
	move.l	$C(a5),a3
	tst.b	$2C(a3)
	beq.b	L22
	move.w	-$C(a5),d7
	add.w	d7,-4(a5)
	move.w	-$A(a5),d7
	cmp.w	-2(a5),d7
	ble.b	L20
	move.w	-$A(a5),-2(a5)
L21
	bra.b	L20
L22
	move.w	-$A(a5),d7
	add.w	d7,-2(a5)
	move.w	-$C(a5),d7
	cmp.w	-4(a5),d7
	ble.b	L20
	move.w	-$C(a5),-4(a5)
L23
L24
L20
	addq.l	#1,d2
L19
	move.l	$C(a5),a3
	cmp.l	$28(a3),d2
	blt.b	L18
	move.l	$8(a5),a3
	move.l	a3,a6
	move.l	a6,a2
	lea	-4(a5),a1
	move.l	(a1)+,(a2)+
	movem.l	(a7)+,d2/d7/a2/a3/a6
	unlk	a5
	rts

	XDEF	aqMin__ss
aqMin__ss
L62	EQU	$8
L63	EQU	$C
	movem.l	d2/d3,-(a7)
	move.w	L62+6(a7),d2
	move.w	L62+4(a7),d3
	cmp.w	d2,d3
	bgt.b	L25
	move.w	d3,d0
	movem.l	(a7)+,d2/d3
	rts
L25
	move.w	d2,d0
	movem.l	(a7)+,d2/d3
	rts

	XDEF	layout__AQLayout__TRC06tPoint
layout__AQLayout__TRC06tPoint
L64	EQU	0
L65	EQU	0
	rts

	XDEF	layout__AQLayout__TRC06tPoint
layout__AQLayout__TRC06tPoint
L66	EQU	0
L67	EQU	0
	rts

	END

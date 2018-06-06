#include "libmftypes.h"
#include "libmfradio.h"

#define RADIO 5043

#include "radiodefs.h"

#if defined(SDCC)

__reentrantb void radio_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant __naked
{
	ptr;
	len;
        __asm;
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	jz	nodata$
	mov	r7,a
	jb	_B_7,codeptr$		; >0x80 code
	jnb	_B_6,xdataptr$		; <0x40 far
	mov	r0,dpl
	mov	dptr,#(AX8052_RADIOBASE | FDATA)
	jb	_B_5,pdataptr$		; >0x60 pdata
idataloop$:
	movx	a,@dptr
	mov	@r0,a
	inc	r0
	djnz	r7,idataloop$
nodata$:
	ret
pdataptr$:
pdataloop$:
	movx	a,@dptr
	movx	@r0,a
	inc	r0
	djnz	r7,pdataloop$
	ret
xdataptr$:
	mov	a,#0x80
	anl	a,_IE
	mov	r5,a
	clr	_EA
	mov	r6,_XPAGE
	mov	_XPAGE,#((AX8052_RADIOBASE | FDATA) >> 8)
	mov	r0,#(AX8052_RADIOBASE | FDATA)
xdataloop$:
	movx	a,@r0
	movx	@dptr,a
	inc	dptr
	djnz	r7,xdataloop$
	mov	_XPAGE,r6
	mov	a,r5
	orl	_IE,a
	ret
codeptr$:
	mov	a,#0x80
	anl	a,_IE
	mov	r5,a
	clr	_EA
	mov	r6,_XPAGE
	mov	_XPAGE,#((AX8052_RADIOBASE | FDATA) >> 8)
	mov	r0,#(AX8052_RADIOBASE | FDATA)
codeloop$:
	movx	a,@r0
	;movc	@dptr,a
	inc	dptr
	djnz	r7,codeloop$
	mov	_XPAGE,r6
	mov	a,r5
	orl	_IE,a
	ret
        __endasm;
}

#else

__reentrantb void radio_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant
{
	if (!len)
		return;
	do {
		*ptr++ = *(const uint8_t __xdata *)(AX8052_RADIOBASE | FDATA);
	} while (--len);
}

#endif

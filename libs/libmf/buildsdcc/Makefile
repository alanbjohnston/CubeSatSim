TARGET_ASM	:= sdcc-asx8051
TARGET_LD	:= sdcc-sdcc
TARGET_CC	:= sdcc-sdcc
TARGET_AR	:= sdcc-sdcclib
TARGET_MODEL	:= --model-small
TARGET_ASMFLAGS	:= -xlosg
TARGET_LNKFLAGS	:= -mmcs51 --debug --code-loc 0x0000 --code-size 0x2000 --xram-loc 0x0000 --xram-size 0x100 $(TARGET_MODEL) --data-loc 0x08
TARGET_CFLAGS	:= --debug -I../source $(TARGET_LNKFLAGS)
LIBSUFFIX	:=

LIBMFOBJ	:= lcdinit.rel lcdsetpos.rel lcdwrstr.rel lcdclear.rel lcdclrdisp.rel lcdwru16.rel lcdwru32.rel \
		   lcdwrhexu16.rel lcdwrhexu32.rel lcduwrnum16.rel lcduwrnum32.rel lcduwrhex16.rel lcduwrhex32.rel \
		   dbglink.rel dbglnktxbuf.rel dbglnkrxbuf.rel dbglnktx.rel dbglnkrx.rel dbglnkwrhexu16.rel dbglnkwrhexu32.rel dbglnkwrstr.rel \
		   dbglnkwru16.rel dbglnkwru32.rel dbglnkwrnum16.rel dbglnkwrnum32.rel dbglnkwrhex16.rel dbglnkwrhex32.rel \
		   crc8ccitt.rel crc8onewire.rel crc8tccitt.rel crc8tccittmsb.rel crc8tonewire.rel crc8tonewiremsb.rel \
		   crc8ccittb.rel crc8ccittmsbb.rel crc8onewireb.rel crc8onewiremsbb.rel \
		   crc8ccitttable.rel crc8onewiretable.rel crc8ccittmsbtable.rel crc8onewiremsbtable.rel \
		   crcccitt.rel crcccittmsb.rel crc16ansi.rel crc16ansimsb.rel crc16dnp.rel crc16dnpmsb.rel crc32ansi.rel crc32ansimsb.rel \
		   crcccittb.rel crcccittmsbb.rel crc16ansib.rel crc16ansimsbb.rel crc16dnpb.rel crc16dnpmsbb.rel crc32ansib.rel crc32ansimsbb.rel \
		   crcccitttable.rel crc16table.rel crc16dnptable.rel crcccittmsbtable.rel crc16msbtable.rel \
		   crc16dnpmsbtable.rel crc32table.rel crc32msbtable.rel pn9.rel pn9table.rel pn9bit.rel pn9bits.rel pn9byte.rel pn9buf.rel \
		   pn15advtable.rel pn15outtable.rel pn15adv.rel pn15out.rel \
		   rev8.rel hweight8.rel hweight16.rel hweight32.rel signext12.rel signext16.rel signext20.rel signext24.rel \
		   chksgnlim16.rel sgnlim16.rel chksgnlim32.rel sgnlim32.rel grayenc8.rel graydec8.rel fmemset.rel fmemcpy.rel \
		   delay.rel random.rel sleep.rel deepsleep.rel sleepcont.rel standby.rel resetcpu.rel \
		   flashunlock.rel flashlock.rel flashwait.rel flashpgerase.rel flashwrite.rel flashread.rel flashcal.rel flashcsec.rel \
		   uarttimer0.rel uarttimer1.rel uarttimer2.rel uart0init.rel uart1init.rel uart0stop.rel uart1stop.rel \
		   uart0txbuf.rel uart1txbuf.rel uart0rxbuf.rel uart1rxbuf.rel \
		   uart0tx.rel uart1tx.rel uart0rx.rel uart1rx.rel uart0wrhexu16.rel uart1wrhexu16.rel uart0wrhexu32.rel uart1wrhexu32.rel \
		   uart0wrstr.rel uart1wrstr.rel uart0wru16.rel uart1wru16.rel uart0wru32.rel uart1wru32.rel \
		   uart0wrnum16.rel uart0wrnum32.rel uart0wrhex16.rel uart0wrhex32.rel \
		   uart1wrnum16.rel uart1wrnum32.rel uart1wrhex16.rel uart1wrhex32.rel \
		   adctemp.rel adccal.rel adccalg.rel adccalt.rel adcuncal.rel adcseoffs00.rel adcseoffs01.rel adcseoffs10.rel \
		   bch3121dec.rel bch3121decp.rel bch3121enc.rel bch3121encp.rel bch3121stab.rel bch3121syn.rel \
		   wrnum16.rel wrnum32.rel offxosc.rel offlpxosc.rel setuplpxosc.rel setupxosc.rel setupcal.rel \
		   wtimer.rel wtrem.rel wtcbadd.rel wtcbrem.rel wt0setcfg.rel wt1setcfg.rel wtstdby.rel \
		   wt0adda.rel wt1adda.rel wt0addr.rel wt1addr.rel wt0curt.rel wt1curt.rel wt0rem.rel wt1rem.rel wt01rem.rel \
		   radiord16.rel radiord24.rel radiord32.rel radiowr16.rel radiowr24.rel radiowr32.rel radiodswakecore.rel \
		   ax5031comminit.rel ax5031commslpexit.rel ax5031reset.rel ax5031deepsleep.rel ax5031rclkena.rel ax5031rclkdis.rel \
		   ax5031rdfifo.rel ax5031wrfifo.rel ax5031regs.rel \
		   ax5042comminit.rel ax5042commslpexit.rel ax5042reset.rel ax5042deepsleep.rel ax5042rclkena.rel ax5042rclkdis.rel \
		   ax5042rdfifo.rel ax5042wrfifo.rel ax5042regs.rel \
		   ax5043comminit.rel ax5043commslpexit.rel ax5043reset.rel ax5043deepsleep.rel ax5043rclkena.rel ax5043rclkdis.rel \
		   ax5043rdfifo.rel ax5043wrfifo.rel ax5043regs.rel \
		   ax5051comminit.rel ax5051commslpexit.rel ax5051reset.rel ax5051deepsleep.rel ax5051rclkena.rel ax5051rclkdis.rel \
		   ax5051rdfifo.rel ax5051wrfifo.rel ax5051regs.rel \
		   ax8052regs.rel

BINARIES	:=

all:				libmf.lib libmflarge.lib $(BINARIES) $(patsubst %.omf,%.cdb,$(BINARIES)) $(patsubst %.omf,%.ihx,$(BINARIES))

clean:
				rm -rf *.lnk libmf.lib libmf libmflarge.lib libmflarge mflibbinsdcc.tar.gz mflibsrc.tar.gz

tar:				mflibbinsdcc.tar.gz mflibsrc.tar.gz

mflibbinsdcc.tar.gz:		libmf.lib libmflarge.lib
				tar -c -v -z -f $@ libmf.lib libmflarge.lib

mflibsrc.tar.gz:
				cd .. ; \
				tar -c -v -z -f buildsdcc/$@ build*/Makefile source/*.c source/*.h source/*.s51 doc/LibMF.pdf buildiar/genrregs.pl

ifeq ($(LIBSUFFIX),)
libmflarge.lib:
				make TARGET_MODEL=--model-large LIBSUFFIX=large
endif

libmf$(LIBSUFFIX):
				[ ! -d $@ ] && mkdir -p $@

.PRECIOUS:			%.rel %.omf %.ihx %.cdb

%.rel:				../%.c
				($(TARGET_CC) -c $(TARGET_CFLAGS) -o $@ $<; x=$$?; dn=`dirname $<`/; fn=`basename $< .c`; rm -f $${dn}$${fn}.asm ; exit $${x})

%.omf %.ihx %.cdb:		%.rel libmf$(LIBSUFFIX).lib
				(libdir=`pwd`; dn=`dirname $<`/; fn=`basename $< .rel`; cd $${dn}; $(TARGET_LD) -V $(TARGET_LNKFLAGS) -L$${libdir} -llibmf$(LIBSUFFIX) $${fn}.rel; x=$$?; mv $${fn} $${fn}.omf; exit $${x})

libmf$(LIBSUFFIX)/%.rel:		../source/%.c | libmf$(LIBSUFFIX)
					($(TARGET_CC) -c $(TARGET_CFLAGS) -o $@ $<; x=$$?; dn=`dirname $<`/; fn=`basename $< .c`; rm -f $${dn}$${fn}.asm ; exit $${x})

libmf$(LIBSUFFIX)/%.rel:		libmf$(LIBSUFFIX)/%.c | libmf$(LIBSUFFIX)
					($(TARGET_CC) -c $(TARGET_CFLAGS) -o $@ $<; x=$$?; dn=`dirname $<`/; fn=`basename $< .c`; rm -f $${dn}$${fn}.asm ; exit $${x})

libmf$(LIBSUFFIX).lib:			$(patsubst %.rel,libmf$(LIBSUFFIX)/%.rel,$(LIBMFOBJ)) | libmf$(LIBSUFFIX)
					$(TARGET_AR) -a $@ $(patsubst %.rel,libmf$(LIBSUFFIX)/%.rel,$(LIBMFOBJ))

libmf$(LIBSUFFIX)/uarttimer0.c:		../source/uarttimer.c
					(unifdef -DTIMER=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uarttimer1.c:		../source/uarttimer.c
					(unifdef -DTIMER=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uarttimer2.c:		../source/uarttimer.c
					(unifdef -DTIMER=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart0init.c:		../source/uartinit.c
					(unifdef -DUART=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart0stop.c:		../source/uartstop.c
					(unifdef -DUART=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart1init.c:		../source/uartinit.c
					(unifdef -DUART=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart1stop.c:		../source/uartstop.c
					(unifdef -DUART=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart0%.c:		../source/io%.c
					(unifdef -DUART=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/uart1%.c:		../source/io%.c
					(unifdef -DUART=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/dbglnk%.c:		../source/io%.c
					(unifdef -DUART=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/lcdu%.c:		../source/io%.c
					(unifdef -DUART=3 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/ax5031%.c:		../source/radio%.c
					(unifdef -DRADIO=5031 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/ax5042%.c:		../source/radio%.c
					(unifdef -DRADIO=5042 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/ax5043%.c:		../source/radio%.c
					(unifdef -DRADIO=5043 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/ax5051%.c:		../source/radio%.c
					(unifdef -DRADIO=5051 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8tccitt.c:		../source/crc8.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8tccittmsb.c:	../source/crc8msb.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8tonewire.c:	../source/crc8.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8tonewiremsb.c:	../source/crc8msb.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16ansi.c:		../source/crc16.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16ansimsb.c:	../source/crc16msb.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16dnp.c:		../source/crc16.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16dnpmsb.c:	../source/crc16msb.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crcccitt.c:		../source/crc16.c
					(unifdef -DCRCMODE=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crcccittmsb.c:	../source/crc16msb.c
					(unifdef -DCRCMODE=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc32ansi.c:		../source/crc32.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc32ansimsb.c:	../source/crc32msb.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8ccittb.c:		../source/crc8b.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8ccittmsbb.c:	../source/crc8b.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8onewireb.c:	../source/crc8b.c
					(unifdef -DCRCMODE=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc8onewiremsbb.c:	../source/crc8b.c
					(unifdef -DCRCMODE=3 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16ansib.c:		../source/crc16b.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16ansimsbb.c:	../source/crc16b.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16dnpb.c:		../source/crc16b.c
					(unifdef -DCRCMODE=2 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc16dnpmsbb.c:	../source/crc16b.c
					(unifdef -DCRCMODE=3 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crcccittb.c:		../source/crc16b.c
					(unifdef -DCRCMODE=4 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crcccittmsbb.c:	../source/crc16b.c
					(unifdef -DCRCMODE=5 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc32ansib.c:		../source/crc32b.c
					(unifdef -DCRCMODE=0 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/crc32ansimsbb.c:	../source/crc32b.c
					(unifdef -DCRCMODE=1 -o $@ $<; x=$$?; if [ $${x} -lt 2 ] ; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/lcdinit.rel:		../source/lcdinit.c ../source/libmflcd.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdsetpos.rel:	../source/lcdsetpos.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrstr.rel:		../source/lcdwrstr.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclear.rel:		../source/lcdclear.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclrdisp.rel:	../source/lcdclrdisp.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru16.rel:		../source/lcdwru16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru32.rel:		../source/lcdwru32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu16.rel:	../source/lcdwrhexu16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu32.rel:	../source/lcdwrhexu32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum16.rel:	libmf$(LIBSUFFIX)/lcduwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum32.rel:	libmf$(LIBSUFFIX)/lcduwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex16.rel:	libmf$(LIBSUFFIX)/lcduwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex32.rel:	libmf$(LIBSUFFIX)/lcduwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/delay.rel:		../source/delay.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/random.rel:		../source/random.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitt.rel:	../source/crc8ccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewire.rel:	../source/crc8onewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitttable.rel:	../source/crc8ccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiretable.rel:	../source/crc8onewiretable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbtable.rel:	../source/crc8ccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbtable.rel:	../source/crc8onewiremsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitttable.rel:	../source/crcccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16table.rel:	../source/crc16table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnptable.rel:	../source/crc16dnptable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbtable.rel:	../source/crcccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16msbtable.rel:	../source/crc16msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbtable.rel:	../source/crc16dnpmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32table.rel:	../source/crc32table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32msbtable.rel:	../source/crc32msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccitt.rel:	libmf$(LIBSUFFIX)/crc8tccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccittmsb.rel:	libmf$(LIBSUFFIX)/crc8tccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewire.rel:	libmf$(LIBSUFFIX)/crc8tonewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewiremsb.rel:	libmf$(LIBSUFFIX)/crc8tonewiremsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansi.rel:	libmf$(LIBSUFFIX)/crc16ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsb.rel:	libmf$(LIBSUFFIX)/crc16ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnp.rel:		libmf$(LIBSUFFIX)/crc16dnp.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsb.rel:	libmf$(LIBSUFFIX)/crc16dnpmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitt.rel:		libmf$(LIBSUFFIX)/crcccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsb.rel:	libmf$(LIBSUFFIX)/crcccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansi.rel:	libmf$(LIBSUFFIX)/crc32ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsb.rel:	libmf$(LIBSUFFIX)/crc32ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittb.rel:	libmf$(LIBSUFFIX)/crc8ccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbb.rel:	libmf$(LIBSUFFIX)/crc8ccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewireb.rel:	libmf$(LIBSUFFIX)/crc8onewireb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbb.rel:	libmf$(LIBSUFFIX)/crc8onewiremsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansib.rel:	libmf$(LIBSUFFIX)/crc16ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsbb.rel:	libmf$(LIBSUFFIX)/crc16ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpb.rel:	libmf$(LIBSUFFIX)/crc16dnpb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbb.rel:	libmf$(LIBSUFFIX)/crc16dnpmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittb.rel:	libmf$(LIBSUFFIX)/crcccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbb.rel:	libmf$(LIBSUFFIX)/crcccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansib.rel:	libmf$(LIBSUFFIX)/crc32ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsbb.rel:	libmf$(LIBSUFFIX)/crc32ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9.rel:		../source/pn9.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9table.rel:		../source/pn9table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bit.rel:		../source/pn9bit.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bits.rel:		../source/pn9bits.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9byte.rel:		../source/pn9byte.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9buf.rel:		../source/pn9buf.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15advtable.rel:	../source/pn15advtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15outtable.rel:	../source/pn15outtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15adv.rel:		../source/pn15adv.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15out.rel:		../source/pn15out.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/rev8.rel:		../source/rev8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight8.rel:		../source/hweight8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight16.rel:	../source/hweight16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight32.rel:	../source/hweight32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext12.rel:	../source/signext12.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext16.rel:	../source/signext16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext20.rel:	../source/signext20.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext24.rel:	../source/signext24.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim16.rel:	../source/chksgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim16.rel:		../source/sgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim32.rel:	../source/chksgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim32.rel:		../source/sgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/grayenc8.rel:		../source/grayenc8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/graydec8.rel:		../source/graydec8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemset.rel:		../source/fmemset.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemcpy.rel:		../source/fmemcpy.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/standby.rel:		../source/standby.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleep.rel:		../source/sleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/deepsleep.rel:	../source/deepsleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleepcont.rel:	../source/sleepcont.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/resetcpu.rel:		../source/resetcpu.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashunlock.rel:	../source/flashunlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashlock.rel:	../source/flashlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwait.rel:	../source/flashwait.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashpgerase.rel:	../source/flashpgerase.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwrite.rel:	../source/flashwrite.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashread.rel:	../source/flashread.c ../source/libmfflash.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcal.rel:		../source/flashcal.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcsec.rel:	../source/flashcsec.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0init.rel:	libmf$(LIBSUFFIX)/uart0init.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1init.rel:	libmf$(LIBSUFFIX)/uart1init.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0stop.rel:	libmf$(LIBSUFFIX)/uart0stop.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1stop.rel:	libmf$(LIBSUFFIX)/uart1stop.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0txbuf.rel:	libmf$(LIBSUFFIX)/uart0txbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1txbuf.rel:	libmf$(LIBSUFFIX)/uart1txbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rxbuf.rel:	libmf$(LIBSUFFIX)/uart0rxbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rxbuf.rel:	libmf$(LIBSUFFIX)/uart1rxbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0tx.rel:		libmf$(LIBSUFFIX)/uart0tx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1tx.rel:		libmf$(LIBSUFFIX)/uart1tx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rx.rel:		libmf$(LIBSUFFIX)/uart0rx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rx.rel:		libmf$(LIBSUFFIX)/uart1rx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu16.rel:	libmf$(LIBSUFFIX)/uart0wrhexu16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu16.rel:	libmf$(LIBSUFFIX)/uart1wrhexu16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu32.rel:	libmf$(LIBSUFFIX)/uart0wrhexu32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu32.rel:	libmf$(LIBSUFFIX)/uart1wrhexu32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrstr.rel:	libmf$(LIBSUFFIX)/uart0wrstr.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrstr.rel:	libmf$(LIBSUFFIX)/uart1wrstr.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru16.rel:	libmf$(LIBSUFFIX)/uart0wru16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru16.rel:	libmf$(LIBSUFFIX)/uart1wru16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru32.rel:	libmf$(LIBSUFFIX)/uart0wru32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru32.rel:	libmf$(LIBSUFFIX)/uart1wru32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum16.rel:	libmf$(LIBSUFFIX)/uart0wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum16.rel:	libmf$(LIBSUFFIX)/uart1wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum32.rel:	libmf$(LIBSUFFIX)/uart0wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum32.rel:	libmf$(LIBSUFFIX)/uart1wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex16.rel:	libmf$(LIBSUFFIX)/uart0wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex16.rel:	libmf$(LIBSUFFIX)/uart1wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex32.rel:	libmf$(LIBSUFFIX)/uart0wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex32.rel:	libmf$(LIBSUFFIX)/uart1wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglink.rel:		../source/dbglink.c ../source/libmfdbglink.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktxbuf.rel:	libmf$(LIBSUFFIX)/dbglnktxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrxbuf.rel:	libmf$(LIBSUFFIX)/dbglnkrxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktx.rel:		libmf$(LIBSUFFIX)/dbglnktx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrx.rel:		libmf$(LIBSUFFIX)/dbglnkrx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu16.rel:	libmf$(LIBSUFFIX)/dbglnkwrhexu16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu32.rel:	libmf$(LIBSUFFIX)/dbglnkwrhexu32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrstr.rel:	libmf$(LIBSUFFIX)/dbglnkwrstr.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru16.rel:	libmf$(LIBSUFFIX)/dbglnkwru16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru32.rel:	libmf$(LIBSUFFIX)/dbglnkwru32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum16.rel:	libmf$(LIBSUFFIX)/dbglnkwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum32.rel:	libmf$(LIBSUFFIX)/dbglnkwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex16.rel:	libmf$(LIBSUFFIX)/dbglnkwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex32.rel:	libmf$(LIBSUFFIX)/dbglnkwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adctemp.rel:		../source/adctemp.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccal.rel:		../source/adccal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalg.rel:		../source/adccalg.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalt.rel:		../source/adccalt.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcuncal.rel:		../source/adcuncal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs00.rel:	../source/adcseoffs00.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs01.rel:	../source/adcseoffs01.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs10.rel:	../source/adcseoffs10.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121dec.rel:	../source/bch3121dec.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121decp.rel:	../source/bch3121decp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121enc.rel:	../source/bch3121enc.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121encp.rel:	../source/bch3121encp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121stab.rel:	../source/bch3121stab.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121syn.rel:	../source/bch3121syn.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum16.rel:		../source/wrnum16.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum32.rel:		../source/wrnum32.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offxosc.rel:		../source/offxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offlpxosc.rel:	../source/offlpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setuplpxosc.rel:	../source/setuplpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupxosc.rel:	../source/setupxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupcal.rel:		../source/setupcal.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtimer.rel:		../source/wtimer.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtrem.rel:		../source/wtrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbadd.rel:		../source/wtcbadd.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbrem.rel:		../source/wtcbrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0setcfg.rel:	../source/wt0setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1setcfg.rel:	../source/wt1setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtstdby.rel:		../source/wtstdby.c ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0adda.rel:		../source/wt0adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1adda.rel:		../source/wt1adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0addr.rel:		../source/wt0addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1addr.rel:		../source/wt1addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0curt.rel:		../source/wt0curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1curt.rel:		../source/wt1curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0rem.rel:		../source/wt0rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1rem.rel:		../source/wt1rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt01rem.rel:		../source/wt01rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord16.rel:	../source/radiord16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord24.rel:	../source/radiord24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord32.rel:	../source/radiord32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr16.rel:	../source/radiowr16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr24.rel:	../source/radiowr24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr32.rel:	../source/radiowr32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiodswakecore.rel:	../source/radiodswakecore.c ../source/radiodefs.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031comminit.rel:	libmf$(LIBSUFFIX)/ax5031comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031commslpexit.rel:	libmf$(LIBSUFFIX)/ax5031commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031reset.rel:	libmf$(LIBSUFFIX)/ax5031reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031deepsleep.rel:	libmf$(LIBSUFFIX)/ax5031deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkena.rel:	../source/ax5031rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkdis.rel:	../source/ax5031rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rdfifo.rel:	libmf$(LIBSUFFIX)/ax5031rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031wrfifo.rel:	libmf$(LIBSUFFIX)/ax5031wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031regs.rel:	../source/ax5031regs.c ../source/ax8052f131.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042comminit.rel:	libmf$(LIBSUFFIX)/ax5042comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042commslpexit.rel:	libmf$(LIBSUFFIX)/ax5042commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042reset.rel:	libmf$(LIBSUFFIX)/ax5042reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042deepsleep.rel:	libmf$(LIBSUFFIX)/ax5042deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkena.rel:	../source/ax5042rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkdis.rel:	../source/ax5042rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rdfifo.rel:	libmf$(LIBSUFFIX)/ax5042rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042wrfifo.rel:	libmf$(LIBSUFFIX)/ax5042wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042regs.rel:	../source/ax5042regs.c ../source/ax8052f142.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043comminit.rel:	libmf$(LIBSUFFIX)/ax5043comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043commslpexit.rel:	libmf$(LIBSUFFIX)/ax5043commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043reset.rel:	libmf$(LIBSUFFIX)/ax5043reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043deepsleep.rel:	libmf$(LIBSUFFIX)/ax5043deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkena.rel:	../source/ax5043rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkdis.rel:	../source/ax5043rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rdfifo.rel:	libmf$(LIBSUFFIX)/ax5043rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043wrfifo.rel:	libmf$(LIBSUFFIX)/ax5043wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043regs.rel:	../source/ax5043regs.c ../source/ax8052f143.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051comminit.rel:	libmf$(LIBSUFFIX)/ax5051comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051commslpexit.rel:	libmf$(LIBSUFFIX)/ax5051commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051reset.rel:	libmf$(LIBSUFFIX)/ax5051reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051deepsleep.rel:	libmf$(LIBSUFFIX)/ax5051deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkena.rel:	../source/ax5051rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkdis.rel:	../source/ax5051rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rdfifo.rel:	libmf$(LIBSUFFIX)/ax5051rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051wrfifo.rel:	libmf$(LIBSUFFIX)/ax5051wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051regs.rel:	../source/ax5051regs.c ../source/ax8052f151.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax8052regs.rel:	../source/ax8052regs.c ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)

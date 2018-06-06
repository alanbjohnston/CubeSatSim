TARGET_MODEL	:= SMALL
LIBSUFFIX	:=
KEILPATH	:= C:/EDA/Keil
C51INC		:= $(KEILPATH)/C51/INC
C51LIB		:= $(KEILPATH)/C51/LIB
C51FLAGS	:= OMF2 OBJECTADVANCED $(TARGET_MODEL) DEBUG INCDIR "($(shell cygpath -w ../source))"
A51OPT		:= "SET ($(TARGET_MODEL))" EP DEBUG
L51OPT		:= "CLASSES (XDATA (X:0x0-X:0x1FFF), HDATA (X:0x0-X:0x1FFF), CODE (C:0x0-C:0xFFFF), CONST (C:0x0-C:0xFFFF), ECODE (C:0x0-C:0xFFFF), HCONST (C:0x0-C:0xFFFF))"
CPU_TYPE	:= 8052AH
CPU_VENDOR	:= Intel
CPU_XTAL	:= 0x01312D00

LIBMFOBJ        := lcdinit.obj lcdsetpos.obj lcdwrstr.obj lcdclear.obj lcdclrdisp.obj lcdwru16.obj lcdwru32.obj \
		   lcdwrhexu16.obj lcdwrhexu32.obj lcduwrnum16.obj lcduwrnum32.obj lcduwrhex16.obj lcduwrhex32.obj \
                   dbglink.obj dbglnktxbuf.obj dbglnkrxbuf.obj dbglnktx.obj dbglnkrx.obj dbglnkwrhexu16.obj dbglnkwrhexu32.obj dbglnkwrstr.obj \
		   dbglnkwru16.obj dbglnkwru32.obj dbglnkwrnum16.obj dbglnkwrnum32.obj dbglnkwrhex16.obj dbglnkwrhex32.obj \
		   crc8ccitt.obj crc8onewire.obj crc8tccitt.obj crc8tccittmsb.obj crc8tonewire.obj crc8tonewiremsb.obj \
		   crc8ccittb.obj crc8ccittmsbb.obj crc8onewireb.obj crc8onewiremsbb.obj \
		   crc8ccitttable.obj crc8onewiretable.obj crc8ccittmsbtable.obj crc8onewiremsbtable.obj \
                   crcccitt.obj crcccittmsb.obj crc16ansi.obj crc16ansimsb.obj crc16dnp.obj crc16dnpmsb.obj crc32ansi.obj crc32ansimsb.obj \
		   crcccittb.obj crcccittmsbb.obj crc16ansib.obj crc16ansimsbb.obj crc16dnpb.obj crc16dnpmsbb.obj crc32ansib.obj crc32ansimsbb.obj \
		   crcccitttable.obj crc16table.obj crc16dnptable.obj crcccittmsbtable.obj crc16msbtable.obj \
		   crc16dnpmsbtable.obj crc32table.obj crc32msbtable.obj pn9.obj pn9table.obj pn9bit.obj pn9bits.obj pn9byte.obj pn9buf.obj \
		   pn15advtable.obj pn15outtable.obj pn15adv.obj pn15out.obj \
		   rev8.obj hweight8.obj hweight16.obj hweight32.obj signext12.obj signext16.obj signext20.obj signext24.obj \
		   chksgnlim16.obj sgnlim16.obj chksgnlim32.obj sgnlim32.obj grayenc8.obj graydec8.obj fmemset.obj fmemcpy.obj \
		   delay.obj random.obj sleep.obj sleepcont.obj deepsleep.obj standby.obj resetcpu.obj \
                   flashunlock.obj flashlock.obj flashwait.obj flashpgerase.obj flashwrite.obj flashread.obj flashcal.obj flashcsec.obj \
                   uarttimer0.obj uarttimer1.obj uarttimer2.obj uart0init.obj uart1init.obj uart0txbuf.obj uart1txbuf.obj uart0rxbuf.obj uart1rxbuf.obj \
                   uart0tx.obj uart1tx.obj uart0rx.obj uart1rx.obj uart0wrhexu16.obj uart1wrhexu16.obj uart0wrhexu32.obj uart1wrhexu32.obj \
                   uart0wrstr.obj uart1wrstr.obj uart0wru16.obj uart1wru16.obj uart0wru32.obj uart1wru32.obj \
		   uart0wrnum16.obj uart0wrnum32.obj uart0wrhex16.obj uart0wrhex32.obj \
		   uart1wrnum16.obj uart1wrnum32.obj uart1wrhex16.obj uart1wrhex32.obj \
		   adctemp.obj adccal.obj adccalg.obj adccalt.obj adcuncal.obj adcseoffs00.obj adcseoffs01.obj adcseoffs10.obj \
		   bch3121dec.obj bch3121decp.obj bch3121enc.obj bch3121encp.obj bch3121stab.obj bch3121syn.obj \
		   wrnum16.obj wrnum32.obj offxosc.obj offlpxosc.obj setuplpxosc.obj setupxosc.obj setupcal.obj \
		   wtimer.obj wtrem.obj wtcbadd.obj wtcbrem.obj wt0setcfg.obj wt1setcfg.obj wtstdby.obj \
		   wt0adda.obj wt1adda.obj wt0addr.obj wt1addr.obj wt0curt.obj wt1curt.obj wt0rem.obj wt1rem.obj wt01rem.obj \
		   radiord16.obj radiord24.obj radiord32.obj radiowr16.obj radiowr24.obj radiowr32.obj radiodswakecore.obj \
		   ax5031comminit.obj ax5031commslpexit.obj ax5031reset.obj ax5031deepsleep.obj ax5031rclkena.obj ax5031rclkdis.obj \
		   ax5031rdfifo.obj ax5031wrfifo.obj ax5031regs.obj \
		   ax5042comminit.obj ax5042commslpexit.obj ax5042reset.obj ax5042deepsleep.obj ax5042rclkena.obj ax5042rclkdis.obj \
		   ax5042rdfifo.obj ax5042wrfifo.obj ax5042regs.obj \
		   ax5043comminit.obj ax5043commslpexit.obj ax5043reset.obj ax5043deepsleep.obj ax5043rclkena.obj ax5043rclkdis.obj \
		   ax5043rdfifo.obj ax5043wrfifo.obj ax5043regs.obj \
		   ax5051comminit.obj ax5051commslpexit.obj ax5051reset.obj ax5051deepsleep.obj ax5051rclkena.obj ax5051rclkdis.obj \
		   ax5051rdfifo.obj ax5051wrfifo.obj ax5051regs.obj \
		   ax8052regs.obj

BINARIES	:=

comma		:= ,
empty		:=
space		:= $(empty) $(empty)

all:				libmf.lib libmflarge.lib $(BINARIES) $(patsubst %.omf,%.hex,$(BINARIES))

clean:
				rm -rf LIBMF.LIB libmf.lib libmf LIBMFLARGE.LIB libmflarge.lib libmflarge mflibbinkeil2.tar.gz

tar:				mflibbinkeil2.tar.gz

mflibbinkeil2.tar.gz:
				[ -f LIBMF.LIB ] && mv LIBMF.LIB libmf.lib ; \
				[ -f LIBMFLARGE.LIB ] && mv LIBMFLARGE.LIB libmflarge.lib ; \
				tar -c -v -z -f $@ libmf.lib libmflarge.lib

ifeq ($(LIBSUFFIX),)
libmflarge.lib:
				make TARGET_MODEL=LARGE LIBSUFFIX=large
endif

libmf$(LIBSUFFIX):
				[ ! -d $@ ] && mkdir -p $@

.PRECIOUS:			%.src %.obj %.omf

%.obj:				%.src
				$(KEILPATH)/C51/BIN/AX51.EXE "$(shell cygpath -w $<)" "PR($(shell cygpath -w $(patsubst %.obj,%.ls1,$@)))" $(A51OPT)

%.src:				%.src1
				./fixmodname.pl $@ < $< > $@

%.omf:				%.obj libmf$(LIBSUFFIX).lib $(shell cygpath -u $(C51LIB)/C51S.LIB)
				($(KEILPATH)/C51/BIN/LX51.EXE "$(subst $(space),$(comma),$(foreach fn,$^,$(shell cygpath -w $(fn))))" TO "$(shell cygpath -w $@)" CODE "($(shell cygpath -w $(patsubst %.omf,%.cod,$@)))" $(L51OPT); x=$$?; if [ $${x} -lt 2 ]; then x=0; fi; exit $${x})

%.hex:				%.omf
				$(KEILPATH)/C51/BIN/OHX51.EXE "$(shell cygpath -w $<)" HEXFILE "($(shell cygpath -w $@))"

libmf$(LIBSUFFIX)/%.src:		../source/%.c | libmf$(LIBSUFFIX)
					($(KEILPATH)/C51/BIN/C51.EXE "$(shell cygpath -w $<)" $(C51FLAGS) DEFINE "(AX5043_DISABLE_XSFR,AX5043_DISABLE_NONBLOCKING)" SRC "($(shell cygpath -w $@))" PR "($(shell cygpath -w $(patsubst %.src,%.lst,$@)))"; x=$$?; if [ $${x} -lt 2 ]; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX)/%.src:		libmf$(LIBSUFFIX)/%.c | libmf$(LIBSUFFIX)
					($(KEILPATH)/C51/BIN/C51.EXE "$(shell cygpath -w $<)" $(C51FLAGS) DEFINE "(AX5043_DISABLE_XSFR,AX5043_DISABLE_NONBLOCKING)" SRC "($(shell cygpath -w $@))" PR "($(shell cygpath -w $(patsubst %.src,%.lst,$@)))"; x=$$?; if [ $${x} -lt 2 ]; then x=0; fi; exit $${x})

libmf$(LIBSUFFIX).lib:			$(patsubst %.obj,libmf$(LIBSUFFIX)/%.obj,$(LIBMFOBJ)) | libmf$(LIBSUFFIX)
					$(KEILPATH)/C51/BIN/LIBX51.EXE TRANSFER "$(subst $(space),$(comma),$(foreach fn,$(patsubst %.obj,libmf$(LIBSUFFIX)/%.obj,$(LIBMFOBJ)),$(shell cygpath -w $(fn))))" TO $@

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

libmf$(LIBSUFFIX)/lcdinit.src:		../source/lcdinit.c ../source/libmflcd.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdsetpos.src:	../source/lcdsetpos.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrstr.src:		../source/lcdwrstr.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclear.src:		../source/lcdclear.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclrdisp.src:	../source/lcdclrdisp.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru16.src:		../source/lcdwru16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru32.src:		../source/lcdwru32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu16.src:	../source/lcdwrhexu16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu32.src:	../source/lcdwrhexu32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum16.src:	libmf$(LIBSUFFIX)/lcduwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum32.src:	libmf$(LIBSUFFIX)/lcduwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex16.src:	libmf$(LIBSUFFIX)/lcduwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex32.src:	libmf$(LIBSUFFIX)/lcduwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/delay.src:		../source/delay.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/random.src:		../source/random.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitt.src:	../source/crc8ccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewire.src:	../source/crc8onewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitttable.src:	../source/crc8ccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiretable.src:	../source/crc8onewiretable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbtable.src:	../source/crc8ccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbtable.src:	../source/crc8onewiremsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitttable.src:	../source/crcccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16table.src:	../source/crc16table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnptable.src:	../source/crc16dnptable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbtable.src:	../source/crcccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16msbtable.src:	../source/crc16msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbtable.src:	../source/crc16dnpmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32table.src:	../source/crc32table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32msbtable.src:	../source/crc32msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccitt.src:	libmf$(LIBSUFFIX)/crc8tccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccittmsb.src:	libmf$(LIBSUFFIX)/crc8tccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewire.src:	libmf$(LIBSUFFIX)/crc8tonewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewiremsb.src:	libmf$(LIBSUFFIX)/crc8tonewiremsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansi.src:	libmf$(LIBSUFFIX)/crc16ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsb.src:	libmf$(LIBSUFFIX)/crc16ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnp.src:		libmf$(LIBSUFFIX)/crc16dnp.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsb.src:	libmf$(LIBSUFFIX)/crc16dnpmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitt.src:		libmf$(LIBSUFFIX)/crcccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsb.src:	libmf$(LIBSUFFIX)/crcccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansi.src:	libmf$(LIBSUFFIX)/crc32ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsb.src:	libmf$(LIBSUFFIX)/crc32ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittb.src:	libmf$(LIBSUFFIX)/crc8ccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbb.src:	libmf$(LIBSUFFIX)/crc8ccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewireb.src:	libmf$(LIBSUFFIX)/crc8onewireb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbb.src:	libmf$(LIBSUFFIX)/crc8onewiremsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansib.src:	libmf$(LIBSUFFIX)/crc16ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsbb.src:	libmf$(LIBSUFFIX)/crc16ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpb.src:	libmf$(LIBSUFFIX)/crc16dnpb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbb.src:	libmf$(LIBSUFFIX)/crc16dnpmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittb.src:	libmf$(LIBSUFFIX)/crcccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbb.src:	libmf$(LIBSUFFIX)/crcccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansib.src:	libmf$(LIBSUFFIX)/crc32ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsbb.src:	libmf$(LIBSUFFIX)/crc32ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9.src:		../source/pn9.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9table.src:		../source/pn9table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bit.src:		../source/pn9bit.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bits.src:		../source/pn9bits.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9byte.src:		../source/pn9byte.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9buf.src:		../source/pn9buf.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15advtable.src:	../source/pn15advtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15outtable.src:	../source/pn15outtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15adv.src:		../source/pn15adv.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15out.src:		../source/pn15out.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/rev8.src:		../source/rev8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight8.src:		../source/hweight8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight16.src:	../source/hweight16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight32.src:	../source/hweight32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext12.src:	../source/signext12.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext16.src:	../source/signext16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext20.src:	../source/signext20.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext24.src:	../source/signext24.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim16.src:	../source/chksgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim16.src:		../source/sgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim32.src:	../source/chksgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim32.src:		../source/sgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/grayenc8.src:		../source/grayenc8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/graydec8.src:		../source/graydec8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemset.src:		../source/fmemset.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemcpy.src:		../source/fmemcpy.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/standby.src:		../source/standby.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleep.src:		../source/sleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleepcont.src:	../source/sleepcont.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/deepsleep.src:	../source/deepsleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/resetcpu.src:		../source/resetcpu.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashunlock.src:	../source/flashunlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashlock.src:	../source/flashlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwait.src:	../source/flashwait.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashpgerase.src:	../source/flashpgerase.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwrite.src:	../source/flashwrite.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashread.src:	../source/flashread.c ../source/libmfflash.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcal.src:		../source/flashcal.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcsec.src:	../source/flashcsec.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0init.src:	libmf$(LIBSUFFIX)/uart0init.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1init.src:	libmf$(LIBSUFFIX)/uart1init.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0stop.src:	libmf$(LIBSUFFIX)/uart0stop.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1stop.src:	libmf$(LIBSUFFIX)/uart1stop.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0txbuf.src:	libmf$(LIBSUFFIX)/uart0txbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1txbuf.src:	libmf$(LIBSUFFIX)/uart1txbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rxbuf.src:	libmf$(LIBSUFFIX)/uart0rxbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rxbuf.src:	libmf$(LIBSUFFIX)/uart1rxbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0tx.src:		libmf$(LIBSUFFIX)/uart0tx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1tx.src:		libmf$(LIBSUFFIX)/uart1tx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rx.src:		libmf$(LIBSUFFIX)/uart0rx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rx.src:		libmf$(LIBSUFFIX)/uart1rx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu16.src:	libmf$(LIBSUFFIX)/uart0wrhexu16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu16.src:	libmf$(LIBSUFFIX)/uart1wrhexu16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu32.src:	libmf$(LIBSUFFIX)/uart0wrhexu32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu32.src:	libmf$(LIBSUFFIX)/uart1wrhexu32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrstr.src:	libmf$(LIBSUFFIX)/uart0wrstr.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrstr.src:	libmf$(LIBSUFFIX)/uart1wrstr.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru16.src:	libmf$(LIBSUFFIX)/uart0wru16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru16.src:	libmf$(LIBSUFFIX)/uart1wru16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru32.src:	libmf$(LIBSUFFIX)/uart0wru32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru32.src:	libmf$(LIBSUFFIX)/uart1wru32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum16.src:	libmf$(LIBSUFFIX)/uart0wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum16.src:	libmf$(LIBSUFFIX)/uart1wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum32.src:	libmf$(LIBSUFFIX)/uart0wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum32.src:	libmf$(LIBSUFFIX)/uart1wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex16.src:	libmf$(LIBSUFFIX)/uart0wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex16.src:	libmf$(LIBSUFFIX)/uart1wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex32.src:	libmf$(LIBSUFFIX)/uart0wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex32.src:	libmf$(LIBSUFFIX)/uart1wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglink.src:		../source/dbglink.c ../source/libmfdbglink.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktxbuf.src:	libmf$(LIBSUFFIX)/dbglnktxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrxbuf.src:	libmf$(LIBSUFFIX)/dbglnkrxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktx.src:		libmf$(LIBSUFFIX)/dbglnktx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrx.src:		libmf$(LIBSUFFIX)/dbglnkrx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu16.src:	libmf$(LIBSUFFIX)/dbglnkwrhexu16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu32.src:	libmf$(LIBSUFFIX)/dbglnkwrhexu32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrstr.src:	libmf$(LIBSUFFIX)/dbglnkwrstr.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru16.src:	libmf$(LIBSUFFIX)/dbglnkwru16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru32.src:	libmf$(LIBSUFFIX)/dbglnkwru32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum16.src:	libmf$(LIBSUFFIX)/dbglnkwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum32.src:	libmf$(LIBSUFFIX)/dbglnkwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex16.src:	libmf$(LIBSUFFIX)/dbglnkwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex32.src:	libmf$(LIBSUFFIX)/dbglnkwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adctemp.src:		../source/adctemp.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccal.src:		../source/adccal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalg.src:		../source/adccalg.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalt.src:		../source/adccalt.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcuncal.src:		../source/adcuncal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs00.src:	../source/adcseoffs00.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs01.src:	../source/adcseoffs01.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs10.src:	../source/adcseoffs10.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121dec.src:	../source/bch3121dec.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121decp.src:	../source/bch3121decp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121enc.src:	../source/bch3121enc.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121encp.src:	../source/bch3121encp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121stab.src:	../source/bch3121stab.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121syn.src:	../source/bch3121syn.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum16.src:		../source/wrnum16.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum32.src:		../source/wrnum32.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offxosc.src:		../source/offxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offlpxosc.src:	../source/offlpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setuplpxosc.src:	../source/setuplpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupxosc.src:	../source/setupxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupcal.src:		../source/setupcal.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtimer.src:		../source/wtimer.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtrem.src:		../source/wtrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbadd.src:		../source/wtcbadd.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbrem.src:		../source/wtcbrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0setcfg.src:	../source/wt0setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1setcfg.src:	../source/wt1setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtstdby.src:		../source/wtstdby.c ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0adda.src:		../source/wt0adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1adda.src:		../source/wt1adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0addr.src:		../source/wt0addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1addr.src:		../source/wt1addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0curt.src:		../source/wt0curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1curt.src:		../source/wt1curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0rem.src:		../source/wt0rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1rem.src:		../source/wt1rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt01rem.src:		../source/wt01rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord16.src:	../source/radiord16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord24.src:	../source/radiord24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord32.src:	../source/radiord32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr16.src:	../source/radiowr16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr24.src:	../source/radiowr24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr32.src:	../source/radiowr32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiodswakecore.src:	../source/radiodswakecore.c ../source/radiodefs.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031comminit.src:	libmf$(LIBSUFFIX)/ax5031comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031commslpexit.src:	libmf$(LIBSUFFIX)/ax5031commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031reset.src:	libmf$(LIBSUFFIX)/ax5031reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031deepsleep.src:	libmf$(LIBSUFFIX)/ax5031deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkena.src:	../source/ax5031rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkdis.src:	../source/ax5031rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rdfifo.src:	libmf$(LIBSUFFIX)/ax5031rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031wrfifo.src:	libmf$(LIBSUFFIX)/ax5031wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031regs.src:	../source/ax5031regs.c ../source/ax8052f131.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042comminit.src:	libmf$(LIBSUFFIX)/ax5042comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042commslpexit.src:	libmf$(LIBSUFFIX)/ax5042commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042reset.src:	libmf$(LIBSUFFIX)/ax5042reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042deepsleep.src:	libmf$(LIBSUFFIX)/ax5042deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkena.src:	../source/ax5042rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkdis.src:	../source/ax5042rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rdfifo.src:	libmf$(LIBSUFFIX)/ax5042rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042wrfifo.src:	libmf$(LIBSUFFIX)/ax5042wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042regs.src:	../source/ax5042regs.c ../source/ax8052f142.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043comminit.src:	libmf$(LIBSUFFIX)/ax5043comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043commslpexit.src:	libmf$(LIBSUFFIX)/ax5043commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043reset.src:	libmf$(LIBSUFFIX)/ax5043reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043deepsleep.src:	libmf$(LIBSUFFIX)/ax5043deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkena.src:	../source/ax5043rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkdis.src:	../source/ax5043rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rdfifo.src:	libmf$(LIBSUFFIX)/ax5043rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043wrfifo.src:	libmf$(LIBSUFFIX)/ax5043wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043regs.src:	../source/ax5043regs.c ../source/ax8052f143.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051comminit.src:	libmf$(LIBSUFFIX)/ax5051comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051commslpexit.src:	libmf$(LIBSUFFIX)/ax5051commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051reset.src:	libmf$(LIBSUFFIX)/ax5051reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051deepsleep.src:	libmf$(LIBSUFFIX)/ax5051deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkena.src:	../source/ax5051rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkdis.src:	../source/ax5051rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rdfifo.src:	libmf$(LIBSUFFIX)/ax5051rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051wrfifo.src:	libmf$(LIBSUFFIX)/ax5051wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051regs.src:	../source/ax5051regs.c ../source/ax8052f151.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax8052regs.src:	../source/ax8052regs.c ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)

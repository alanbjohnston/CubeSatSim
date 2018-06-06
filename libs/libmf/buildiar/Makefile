TARGET_MODEL	:= --calling_convention=data_overlay --data_model=small --place_constants=code
TARGET_DEFINES	:= -D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=0
LIBSUFFIX	:=
#TARGET_INSTDIR	:= C:/EDA/IAR\ Systems/Embedded\ Workbench\ 6.0\ Kickstart/8051
#TARGET_INSTDIR	:= C:/EDA/IAR\ Systems/Embedded\ Workbench\ 6.5/8051
#TARGET_INSTDIR	:= C:/Program\ Files\ \(x86\)/IAR\ Systems/Embedded\ Workbench\ 6.5/8051
TARGET_INSTDIR	:= C:/Program\ Files\ \(x86\)/IAR\ Systems/Embedded\ Workbench\ 7.3/8051
TARGET_ASM	:= $(TARGET_INSTDIR)/bin/a8051.exe
TARGET_LD	:= $(TARGET_INSTDIR)/bin/xlink.exe
TARGET_CC	:= $(TARGET_INSTDIR)/bin/icc8051.exe
TARGET_AR	:= $(TARGET_INSTDIR)/bin/xar.exe
TARGET_ASMFLAGS	:= -v0 -D__CORE__=1 -D__CODE_MODEL__=1 -D__NUMBER_OF_DPTRS__=1 $(TARGET_DEFINES) -s+ -M"<>" -r -w+ -I$(TARGET_INSTDIR)/src/lib
TARGET_LNKFLAGS	:= -D_NR_OF_BANKS=0 -D_CODEBANK_END=0 -D_CODEBANK_START=0 -I$(TARGET_INSTDIR)/config -e_small_write=_formatted_write -e_medium_read=_formatted_read -Faomf8051 -Y0 -I$(TARGET_INSTDIR)/lib -f $(TARGET_INSTDIR)/config/devices/_generic/lnk51ew_plain.xcl -D?DPMASK=0x01 -D_NR_OF_VIRTUAL_REGISTERS=8 -D?DPS=0x85 -D?PBANK=0xD9 -D?DPL1=0x84 -D?DPH1=0x85 -s __program_start $(TARGET_INSTDIR)/LIB/CLIB/cl-pli-nsdc-2e16inc.r51 -D_IDATA_STACK_SIZE=0x40 -D_EXTENDED_STACK_START=0x00 -D_EXTENDED_STACK_SIZE=0x00 -D_PDATA_STACK_SIZE=0x80 -D_XDATA_STACK_SIZE=0xEFF -D_XDATA_HEAP_SIZE=0xFF -D_FAR_HEAP_SIZE=0xFFF -D_HUGE_HEAP_SIZE=0xFFF -D_FAR22_HEAP_SIZE=0xFFF 
TARGET_CFLAGS	:= -e --no_unroll --no_inline --no_tbaa --debug --core=plain --dptr=16,1,separate,xor $(TARGET_MODEL) --code_model=near --nr_virtual_regs 8 -Om --library_module --diag_suppress=Pa050 -I../source

LIBMFOBJ	:= lcdinit.r51 lcdsetpos.r51 lcdwrstr.r51 lcdclear.r51 lcdclrdisp.r51 lcdwru16.r51 lcdwru32.r51 \
		   lcdwrhexu16.r51 lcdwrhexu32.r51 lcduwrnum16.r51 lcduwrnum32.r51 lcduwrhex16.r51 lcduwrhex32.r51 \
		   dbglink.r51 dbglnktxbuf.r51 dbglnkrxbuf.r51 dbglnktx.r51 dbglnkrx.r51 dbglnkwrhexu16.r51 dbglnkwrhexu32.r51 dbglnkwrstr.r51 \
		   dbglnkwru16.r51 dbglnkwru32.r51 dbglnkwrnum16.r51 dbglnkwrnum32.r51 dbglnkwrhex16.r51 dbglnkwrhex32.r51 \
		   crc8ccitt.r51 crc8onewire.r51 crc8tccitt.r51 crc8tccittmsb.r51 crc8tonewire.r51 crc8tonewiremsb.r51 \
		   crc8ccittb.r51 crc8ccittmsbb.r51 crc8onewireb.r51 crc8onewiremsbb.r51 \
		   crc8ccitttable.r51 crc8onewiretable.r51 crc8ccittmsbtable.r51 crc8onewiremsbtable.r51 \
		   crcccitt.r51 crcccittmsb.r51 crc16ansi.r51 crc16ansimsb.r51 crc16dnp.r51 crc16dnpmsb.r51 crc32ansi.r51 crc32ansimsb.r51 \
		   crcccittb.r51 crcccittmsbb.r51 crc16ansib.r51 crc16ansimsbb.r51 crc16dnpb.r51 crc16dnpmsbb.r51 crc32ansib.r51 crc32ansimsbb.r51 \
		   crcccitttable.r51 crc16table.r51 crc16dnptable.r51 crcccittmsbtable.r51 crc16msbtable.r51 \
		   crc16dnpmsbtable.r51 crc32table.r51 crc32msbtable.r51 pn9.r51 pn9table.r51 pn9bit.r51 pn9bits.r51 pn9byte.r51 pn9buf.r51 \
		   pn15advtable.r51 pn15outtable.r51 pn15adv.r51 pn15out.r51 \
		   rev8.r51 hweight8.r51 hweight16.r51 hweight32.r51 signext12.r51 signext16.r51 signext20.r51 signext24.r51 \
		   chksgnlim16.r51 sgnlim16.r51 chksgnlim32.r51 sgnlim32.r51 grayenc8.r51 graydec8.r51 fmemsetiar.r51 fmemcpyiar.r51 \
		   delay.r51 random.r51 sleep.r51 sleepcont.r51 deepsleep.r51 standby.r51 resetcpu.r51 \
		   flashunlock.r51 flashlock.r51 flashwait.r51 flashpgerase.r51 flashwrite.r51 flashread.r51 flashcal.r51 flashcsec.r51 \
		   uarttimer0.r51 uarttimer1.r51 uarttimer2.r51 uart0init.r51 uart1init.r51 uart0stop.r51 uart1stop.r51 \
		   uart0txbuf.r51 uart1txbuf.r51 uart0rxbuf.r51 uart1rxbuf.r51 \
		   uart0tx.r51 uart1tx.r51 uart0rx.r51 uart1rx.r51 uart0wrhexu16.r51 uart1wrhexu16.r51 uart0wrhexu32.r51 uart1wrhexu32.r51 \
		   uart0wrstr.r51 uart1wrstr.r51 uart0wru16.r51 uart1wru16.r51 uart0wru32.r51 uart1wru32.r51 \
		   uart0wrnum16.r51 uart0wrnum32.r51 uart0wrhex16.r51 uart0wrhex32.r51 \
		   uart1wrnum16.r51 uart1wrnum32.r51 uart1wrhex16.r51 uart1wrhex32.r51 \
		   adctemp.r51 adccal.r51 adccalg.r51 adccalt.r51 adcuncal.r51 adcseoffs00.r51 adcseoffs01.r51 adcseoffs10.r51 \
		   bch3121dec.r51 bch3121decp.r51 bch3121enc.r51 bch3121encp.r51 bch3121stab.r51 bch3121syn.r51 \
		   wrnum16.r51 wrnum32.r51 offxosc.r51 offlpxosc.r51 setuplpxosc.r51 setupxosc.r51 setupcal.r51 \
		   wtimer.r51 wtrem.r51 wtcbadd.r51 wtcbrem.r51 wt0setcfg.r51 wt1setcfg.r51 wtstdby.r51 \
		   wt0adda.r51 wt1adda.r51 wt0addr.r51 wt1addr.r51 wt0curt.r51 wt1curt.r51 wt0rem.r51 wt1rem.r51 wt01rem.r51 \
		   radiord16.r51 radiord24.r51 radiord32.r51 radiowr16.r51 radiowr24.r51 radiowr32.r51 radiodswakecore.r51 \
		   ax5031comminit.r51 ax5031commslpexit.r51 ax5031reset.r51 ax5031deepsleep.r51 ax5031rclkena.r51 ax5031rclkdis.r51 \
		   ax5031rdfifo.r51 ax5031wrfifo.r51 \
		   ax5042comminit.r51 ax5042commslpexit.r51 ax5042reset.r51 ax5042deepsleep.r51 ax5042rclkena.r51 ax5042rclkdis.r51 \
		   ax5042rdfifo.r51 ax5042wrfifo.r51 \
		   ax5043comminit.r51 ax5043commslpexit.r51 ax5043reset.r51 ax5043deepsleep.r51 ax5043rclkena.r51 ax5043rclkdis.r51 \
		   ax5043rdfifo.r51 ax5043wrfifo.r51 \
		   ax5051comminit.r51 ax5051commslpexit.r51 ax5051reset.r51 ax5051deepsleep.r51 ax5051rclkena.r51 ax5051rclkdis.r51 \
		   ax5051rdfifo.r51 ax5051wrfifo.r51 \
		   ax8052regs.r51 radioregs.r51 CStartup.r51 getpspiar.r51 getxspiar.r51

BINARIES	:=
LIBBINARIES	:= libmf.r51 libmflarge.r51 \
		   libmf-pli-nlpc-1e16x01.r51 libmf-pli-nlpd-1e16x01.r51 libmf-pli-nlxc-1e16x01.r51 libmf-pli-nlxd-1e16x01.r51 \
		   libmf-pli-nsdc-1e16x01.r51 libmf-pli-nsdd-1e16x01.r51 libmf-pli-nsic-1e16x01.r51 libmf-pli-nsid-1e16x01.r51 libmf-pli-nsoc-1e16x01.r51 libmf-pli-nsod-1e16x01.r51 \
		   libmf-pli-ntdc-1e16x01.r51 libmf-pli-ntdd-1e16x01.r51 libmf-pli-ntic-1e16x01.r51 libmf-pli-ntid-1e16x01.r51 libmf-pli-ntoc-1e16x01.r51 libmf-pli-ntod-1e16x01.r51

ifeq ($(LIBSUFFIX),)
all:				$(LIBBINARIES) $(BINARIES) $(patsubst %.omf,%.cdb,$(BINARIES)) $(patsubst %.omf,%.ihx,$(BINARIES))
else
all:				libmf$(LIBSUFFIX).r51
endif

clean:
				rm -rf *.lnk radioregs.s51 $(LIBBINARIES) $(patsubst %.r51,%,$(LIBBINARIES)) mflibbiniar.tar.gz

tar:				mflibbiniar.tar.gz

mflibbiniar.tar.gz:		$(LIBBINARIES)
				tar -c -v -z -f $@ $(LIBBINARIES)

ifeq ($(LIBSUFFIX),)
.PHONY: libmflarge.r51
.PHONY: libmf-pli-nlpc-1e16x01.r51
.PHONY: libmf-pli-nlpd-1e16x01.r51
.PHONY: libmf-pli-nlxc-1e16x01.r51
.PHONY: libmf-pli-nlxd-1e16x01.r51
.PHONY: libmf-pli-nsdc-1e16x01.r51
.PHONY: libmf-pli-nsdd-1e16x01.r51
.PHONY: libmf-pli-nsic-1e16x01.r51
.PHONY: libmf-pli-nsid-1e16x01.r51
.PHONY: libmf-pli-nsoc-1e16x01.r51
.PHONY: libmf-pli-nsod-1e16x01.r51
.PHONY: libmf-pli-ntdc-1e16x01.r51
.PHONY: libmf-pli-ntdd-1e16x01.r51
.PHONY: libmf-pli-ntic-1e16x01.r51
.PHONY: libmf-pli-ntid-1e16x01.r51
.PHONY: libmf-pli-ntoc-1e16x01.r51
.PHONY: libmf-pli-ntod-1e16x01.r51

libmflarge.r51:
	make TARGET_MODEL="--calling_convention=pdata_reentrant --data_model=large --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=2 -D__CALLING_CONVENTION__=3" LIBSUFFIX=large

# name format: -pli-n(1)(2)(3)-1e16x01
# page 152
# (1): data model
#   t - --data_model=tiny         -D__DATA_MODEL__=0
#   s - --data_model=small        -D__DATA_MODEL__=1
#   l - --data_model=large        -D__DATA_MODEL__=2
#   g - --data_model=generic      -D__DATA_MODEL__=3
#   j - --data_model=far_generic  -D__DATA_MODEL__=5
#   f - --data_model=far          -D__DATA_MODEL__=4
# (2): calling convention
#   d - --calling_convention=data_overlay         -D__CALLING_CONVENTION__=0
#   o - --calling_convention=idata_overlay        -D__CALLING_CONVENTION__=1
#   i - --calling_convention=idata_reentrant      -D__CALLING_CONVENTION__=2
#   p - --calling_convention=pdata_reentrant      -D__CALLING_CONVENTION__=3
#   x - --calling_convention=xdata_reentrant      -D__CALLING_CONVENTION__=4
#   e - --calling_convention=ext_stack_reentrant  -D__CALLING_CONVENTION__=5
# (3): constant location
#   c -  --place_constants=code
#   d -  --place_constants=data

libmf-pli-nlpc-1e16x01.r51:
	make TARGET_MODEL="--data_model=large --calling_convention=pdata_reentrant --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=2 -D__CALLING_CONVENTION__=3" LIBSUFFIX=-pli-nlpc-1e16x01

libmf-pli-nlpd-1e16x01.r51:
	make TARGET_MODEL="--data_model=large --calling_convention=pdata_reentrant --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=2 -D__CALLING_CONVENTION__=3" LIBSUFFIX=-pli-nlpd-1e16x01

libmf-pli-nlxc-1e16x01.r51:
	make TARGET_MODEL="--data_model=large --calling_convention=xdata_reentrant --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=2 -D__CALLING_CONVENTION__=4" LIBSUFFIX=-pli-nlxc-1e16x01

libmf-pli-nlxd-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=large --calling_convention=xdata_reentrant --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=2 -D__CALLING_CONVENTION__=4" LIBSUFFIX=-pli-nlxd-1e16x01

libmf-pli-nsdc-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=data_overlay --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=0" LIBSUFFIX=-pli-nsdc-1e16x01

libmf-pli-nsdd-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=data_overlay --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=0" LIBSUFFIX=-pli-nsdd-1e16x01

libmf-pli-nsic-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=idata_reentrant --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=2" LIBSUFFIX=-pli-nsic-1e16x01

libmf-pli-nsid-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=idata_reentrant --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=2" LIBSUFFIX=-pli-nsid-1e16x01

libmf-pli-nsoc-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=idata_overlay --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=1" LIBSUFFIX=-pli-nsoc-1e16x01

libmf-pli-nsod-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=small --calling_convention=idata_overlay --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=1 -D__CALLING_CONVENTION__=1" LIBSUFFIX=-pli-nsod-1e16x01

libmf-pli-ntdc-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=data_overlay --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=0" LIBSUFFIX=-pli-ntdc-1e16x01

libmf-pli-ntdd-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=data_overlay --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=0" LIBSUFFIX=-pli-ntdd-1e16x01

libmf-pli-ntic-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=idata_reentrant --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=2" LIBSUFFIX=-pli-ntic-1e16x01

libmf-pli-ntid-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=idata_reentrant --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=2" LIBSUFFIX=-pli-ntid-1e16x01

libmf-pli-ntoc-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=idata_overlay --place_constants=code" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=1" LIBSUFFIX=-pli-ntoc-1e16x01

libmf-pli-ntod-1e16x01.r51:		      			      
	make TARGET_MODEL="--data_model=tiny --calling_convention=idata_overlay --place_constants=data" TARGET_DEFINES="-D__DATA_MODEL__=0 -D__CALLING_CONVENTION__=1" LIBSUFFIX=-pli-ntod-1e16x01

endif

.PRECIOUS:			radioregs.s51

radioregs.s51:			../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h
				./genrregs.pl $^ > $@

libmf$(LIBSUFFIX):
				[ ! -d $@ ] && mkdir -p $@

.PRECIOUS:			%.r51 %.omf %.ihx %.cdb

%.r51:				../%.c
				$(TARGET_CC) $(TARGET_CFLAGS) -o $@ -lC `basename $@ .r51`.lst $<

%.omf %.ihx %.cdb:		%.r51 libmf$(LIBSUFFIX).r51
				(libdir=`pwd`; dn=`dirname $<`/; fn=`basename $< .r51`; cd $${dn}; $(TARGET_LD) -V $(TARGET_LNKFLAGS) -L$${libdir} -llibmf$(LIBSUFFIX) $${fn}.r51; x=$$?; mv $${fn} $${fn}.omf; exit $${x})

libmf$(LIBSUFFIX)/%.r51:		../source/%.c | libmf$(LIBSUFFIX)
					$(TARGET_CC) $(TARGET_CFLAGS) -o $@ -lC libmf$(LIBSUFFIX)/`basename $@ .r51`.lst $<

libmf$(LIBSUFFIX)/%.r51:		libmf$(LIBSUFFIX)/%.c | libmf$(LIBSUFFIX)
					$(TARGET_CC) $(TARGET_CFLAGS) -o $@ -lC libmf$(LIBSUFFIX)/`basename $@ .r51`.lst $<

libmf$(LIBSUFFIX)/%.r51:		../source/%.s51 | libmf$(LIBSUFFIX)
					$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ -l libmf$(LIBSUFFIX)/`basename $@ .r51`.lst -t8 $<

libmf$(LIBSUFFIX)/%.r51:		%.s51 | libmf$(LIBSUFFIX)
					$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ -l libmf$(LIBSUFFIX)/`basename $@ .r51`.lst -t8 $<

libmf$(LIBSUFFIX)/%.s51:		../source/%.c | libmf$(LIBSUFFIX)
					$(TARGET_CC) $(TARGET_CFLAGS) -o libmf$(LIBSUFFIX)/`basename $@ .s51`.r51 -lA $@ $<

libmf$(LIBSUFFIX)/%.s51:		libmf$(LIBSUFFIX)/%.c | libmf$(LIBSUFFIX)
					$(TARGET_CC) $(TARGET_CFLAGS) -o libmf$(LIBSUFFIX)/`basename $@ .s51`.r51 -lA $@ $<

libmf$(LIBSUFFIX).r51:			$(patsubst %.r51,libmf$(LIBSUFFIX)/%.r51,$(LIBMFOBJ)) | libmf$(LIBSUFFIX)
					$(TARGET_AR) -o $@ $(patsubst %.r51,libmf$(LIBSUFFIX)/%.r51,$(LIBMFOBJ))

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

libmf$(LIBSUFFIX)/lcdinit.r51:		../source/lcdinit.c ../source/libmflcd.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdsetpos.r51:	../source/lcdsetpos.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrstr.r51:		../source/lcdwrstr.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclear.r51:		../source/lcdclear.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdclrdisp.r51:		../source/lcdclrdisp.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru16.r51:		../source/lcdwru16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwru32.r51:		../source/lcdwru32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu16.r51:	../source/lcdwrhexu16.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcdwrhexu32.r51:	../source/lcdwrhexu32.c ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum16.r51:	libmf$(LIBSUFFIX)/lcduwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrnum32.r51:	libmf$(LIBSUFFIX)/lcduwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex16.r51:	libmf$(LIBSUFFIX)/lcduwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/lcduwrhex32.r51:	libmf$(LIBSUFFIX)/lcduwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/delay.r51:		../source/delay.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/random.r51:		../source/random.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitt.r51:	../source/crc8ccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewire.r51:	../source/crc8onewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccitttable.r51:	../source/crc8ccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiretable.r51:	../source/crc8onewiretable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbtable.r51:	../source/crc8ccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbtable.r51:	../source/crc8onewiremsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitttable.r51:	../source/crcccitttable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16table.r51:	../source/crc16table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnptable.r51:	../source/crc16dnptable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbtable.r51:	../source/crcccittmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16msbtable.r51:	../source/crc16msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbtable.r51:	../source/crc16dnpmsbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32table.r51:	../source/crc32table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32msbtable.r51:	../source/crc32msbtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccitt.r51:	libmf$(LIBSUFFIX)/crc8tccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tccittmsb.r51:	libmf$(LIBSUFFIX)/crc8tccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewire.r51:	libmf$(LIBSUFFIX)/crc8tonewire.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8tonewiremsb.r51:	libmf$(LIBSUFFIX)/crc8tonewiremsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansi.r51:	libmf$(LIBSUFFIX)/crc16ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsb.r51:	libmf$(LIBSUFFIX)/crc16ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnp.r51:		libmf$(LIBSUFFIX)/crc16dnp.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsb.r51:	libmf$(LIBSUFFIX)/crc16dnpmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccitt.r51:		libmf$(LIBSUFFIX)/crcccitt.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsb.r51:	libmf$(LIBSUFFIX)/crcccittmsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansi.r51:	libmf$(LIBSUFFIX)/crc32ansi.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsb.r51:	libmf$(LIBSUFFIX)/crc32ansimsb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittb.r51:	libmf$(LIBSUFFIX)/crc8ccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8ccittmsbb.r51:	libmf$(LIBSUFFIX)/crc8ccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewireb.r51:	libmf$(LIBSUFFIX)/crc8onewireb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc8onewiremsbb.r51:	libmf$(LIBSUFFIX)/crc8onewiremsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansib.r51:	libmf$(LIBSUFFIX)/crc16ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16ansimsbb.r51:	libmf$(LIBSUFFIX)/crc16ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpb.r51:	libmf$(LIBSUFFIX)/crc16dnpb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc16dnpmsbb.r51:	libmf$(LIBSUFFIX)/crc16dnpmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittb.r51:	libmf$(LIBSUFFIX)/crcccittb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crcccittmsbb.r51:	libmf$(LIBSUFFIX)/crcccittmsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansib.r51:	libmf$(LIBSUFFIX)/crc32ansib.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/crc32ansimsbb.r51:	libmf$(LIBSUFFIX)/crc32ansimsbb.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9.r51:		../source/pn9.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9table.r51:		../source/pn9table.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bit.r51:		../source/pn9bit.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9bits.r51:		../source/pn9bits.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9byte.r51:		../source/pn9byte.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn9buf.r51:		../source/pn9buf.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15advtable.r51:	../source/pn15advtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15outtable.r51:	../source/pn15outtable.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15adv.r51:		../source/pn15adv.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/pn15out.r51:		../source/pn15out.c ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/rev8.r51:		../source/rev8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight8.r51:		../source/hweight8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight16.r51:	../source/hweight16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/hweight32.r51:	../source/hweight32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext12.r51:	../source/signext12.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext16.r51:	../source/signext16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext20.r51:	../source/signext20.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/signext24.r51:	../source/signext24.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim16.r51:	../source/chksgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim16.r51:		../source/sgnlim16.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/chksgnlim32.r51:	../source/chksgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sgnlim32.r51:		../source/sgnlim32.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/grayenc8.r51:		../source/grayenc8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/graydec8.r51:		../source/graydec8.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemset.r51:		../source/fmemset.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemcpy.r51:		../source/fmemcpy.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemsetiar.r51:	../source/fmemsetiar.s51 | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/fmemcpyiar.r51:	../source/fmemcpyiar.s51 | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/standby.r51:		../source/standby.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleep.r51:		../source/sleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/sleepcont.r51:	../source/sleepcont.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/deepsleep.r51:	../source/deepsleep.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/resetcpu.r51:		../source/resetcpu.c ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashunlock.r51:	../source/flashunlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashlock.r51:	../source/flashlock.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwait.r51:	../source/flashwait.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashpgerase.r51:	../source/flashpgerase.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashwrite.r51:	../source/flashwrite.c ../source/libmfflash.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashread.r51:	../source/flashread.c ../source/libmfflash.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcal.r51:		../source/flashcal.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/flashcsec.r51:	../source/flashcsec.c ../source/libmfcalsector.h ../source/libmfflash.h ../source/libmfcrc.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0init.r51:	libmf$(LIBSUFFIX)/uart0init.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1init.r51:	libmf$(LIBSUFFIX)/uart1init.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0stop.r51:	libmf$(LIBSUFFIX)/uart0stop.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1stop.r51:	libmf$(LIBSUFFIX)/uart1stop.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0txbuf.r51:	libmf$(LIBSUFFIX)/uart0txbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1txbuf.r51:	libmf$(LIBSUFFIX)/uart1txbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rxbuf.r51:	libmf$(LIBSUFFIX)/uart0rxbuf.c ../source/libmfuart0.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rxbuf.r51:	libmf$(LIBSUFFIX)/uart1rxbuf.c ../source/libmfuart1.h ../source/libmfuart.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0tx.r51:		libmf$(LIBSUFFIX)/uart0tx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1tx.r51:		libmf$(LIBSUFFIX)/uart1tx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0rx.r51:		libmf$(LIBSUFFIX)/uart0rx.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1rx.r51:		libmf$(LIBSUFFIX)/uart1rx.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu16.r51:	libmf$(LIBSUFFIX)/uart0wrhexu16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu16.r51:	libmf$(LIBSUFFIX)/uart1wrhexu16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhexu32.r51:	libmf$(LIBSUFFIX)/uart0wrhexu32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhexu32.r51:	libmf$(LIBSUFFIX)/uart1wrhexu32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrstr.r51:	libmf$(LIBSUFFIX)/uart0wrstr.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrstr.r51:	libmf$(LIBSUFFIX)/uart1wrstr.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru16.r51:	libmf$(LIBSUFFIX)/uart0wru16.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru16.r51:	libmf$(LIBSUFFIX)/uart1wru16.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wru32.r51:	libmf$(LIBSUFFIX)/uart0wru32.c ../source/libmfuart0.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wru32.r51:	libmf$(LIBSUFFIX)/uart1wru32.c ../source/libmfuart1.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum16.r51:	libmf$(LIBSUFFIX)/uart0wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum16.r51:	libmf$(LIBSUFFIX)/uart1wrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrnum32.r51:	libmf$(LIBSUFFIX)/uart0wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrnum32.r51:	libmf$(LIBSUFFIX)/uart1wrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex16.r51:	libmf$(LIBSUFFIX)/uart0wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex16.r51:	libmf$(LIBSUFFIX)/uart1wrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart0wrhex32.r51:	libmf$(LIBSUFFIX)/uart0wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/uart1wrhex32.r51:	libmf$(LIBSUFFIX)/uart1wrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglink.r51:		../source/dbglink.c ../source/libmfdbglink.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktxbuf.r51:	libmf$(LIBSUFFIX)/dbglnktxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrxbuf.r51:	libmf$(LIBSUFFIX)/dbglnkrxbuf.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnktx.r51:		libmf$(LIBSUFFIX)/dbglnktx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkrx.r51:		libmf$(LIBSUFFIX)/dbglnkrx.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu16.r51:	libmf$(LIBSUFFIX)/dbglnkwrhexu16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhexu32.r51:	libmf$(LIBSUFFIX)/dbglnkwrhexu32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrstr.r51:	libmf$(LIBSUFFIX)/dbglnkwrstr.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru16.r51:	libmf$(LIBSUFFIX)/dbglnkwru16.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwru32.r51:	libmf$(LIBSUFFIX)/dbglnkwru32.c ../source/libmfdbglink.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum16.r51:	libmf$(LIBSUFFIX)/dbglnkwrnum16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrnum32.r51:	libmf$(LIBSUFFIX)/dbglnkwrnum32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex16.r51:	libmf$(LIBSUFFIX)/dbglnkwrhex16.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/dbglnkwrhex32.r51:	libmf$(LIBSUFFIX)/dbglnkwrhex32.c ../source/wrnum.h ../source/libmfuart0.h ../source/libmfuart1.h ../source/libmfdbglink.h ../source/libmflcd.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adctemp.r51:		../source/adctemp.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccal.r51:		../source/adccal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalg.r51:		../source/adccalg.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adccalt.r51:		../source/adccalt.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcuncal.r51:		../source/adcuncal.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs00.r51:	../source/adcseoffs00.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs01.r51:	../source/adcseoffs01.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/adcseoffs10.r51:	../source/adcseoffs10.c ../source/libmfadc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121dec.r51:	../source/bch3121dec.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121decp.r51:	../source/bch3121decp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121enc.r51:	../source/bch3121enc.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121encp.r51:	../source/bch3121encp.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121stab.r51:	../source/bch3121stab.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/bch3121syn.r51:	../source/bch3121syn.c ../source/libmfbch.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum16.r51:		../source/wrnum16.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wrnum32.r51:		../source/wrnum32.c ../source/wrnum.h ../source/libmftypes.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offxosc.r51:		../source/offxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/offlpxosc.r51:	../source/offlpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setuplpxosc.r51:	../source/setuplpxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupxosc.r51:	../source/setupxosc.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/setupcal.r51:		../source/setupcal.c ../source/libmfosc.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtimer.r51:		../source/wtimer.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtrem.r51:		../source/wtrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbadd.r51:		../source/wtcbadd.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtcbrem.r51:		../source/wtcbrem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0setcfg.r51:	../source/wt0setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1setcfg.r51:	../source/wt1setcfg.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wtstdby.r51:		../source/wtstdby.c ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0adda.r51:		../source/wt0adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1adda.r51:		../source/wt1adda.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0addr.r51:		../source/wt0addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1addr.r51:		../source/wt1addr.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0curt.r51:		../source/wt0curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1curt.r51:		../source/wt1curt.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt0rem.r51:		../source/wt0rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt1rem.r51:		../source/wt1rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/wt01rem.r51:		../source/wt01rem.c ../source/wtimer.h ../source/libmfwtimer.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord16.r51:	../source/radiord16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord24.r51:	../source/radiord24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiord32.r51:	../source/radiord32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr16.r51:	../source/radiowr16.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr24.r51:	../source/radiowr24.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiowr32.r51:	../source/radiowr32.c ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radiodswakecore.r51:	../source/radiodswakecore.c ../source/radiodefs.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031comminit.r51:	libmf$(LIBSUFFIX)/ax5031comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031commslpexit.r51:	libmf$(LIBSUFFIX)/ax5031commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031reset.r51:	libmf$(LIBSUFFIX)/ax5031reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031deepsleep.r51:	libmf$(LIBSUFFIX)/ax5031deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkena.r51:	../source/ax5031rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rclkdis.r51:	../source/ax5031rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031rdfifo.r51:	libmf$(LIBSUFFIX)/ax5031rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031wrfifo.r51:	libmf$(LIBSUFFIX)/ax5031wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5031regs.r51:	../source/ax5031regs.c ../source/ax8052f131.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042comminit.r51:	libmf$(LIBSUFFIX)/ax5042comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042commslpexit.r51:	libmf$(LIBSUFFIX)/ax5042commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042reset.r51:	libmf$(LIBSUFFIX)/ax5042reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042deepsleep.r51:	libmf$(LIBSUFFIX)/ax5042deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkena.r51:	../source/ax5042rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rclkdis.r51:	../source/ax5042rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042rdfifo.r51:	libmf$(LIBSUFFIX)/ax5042rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042wrfifo.r51:	libmf$(LIBSUFFIX)/ax5042wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5042regs.r51:	../source/ax5042regs.c ../source/ax8052f142.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043comminit.r51:	libmf$(LIBSUFFIX)/ax5043comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043commslpexit.r51:	libmf$(LIBSUFFIX)/ax5043commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043reset.r51:	libmf$(LIBSUFFIX)/ax5043reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043deepsleep.r51:	libmf$(LIBSUFFIX)/ax5043deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkena.r51:	../source/ax5043rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rclkdis.r51:	../source/ax5043rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043rdfifo.r51:	libmf$(LIBSUFFIX)/ax5043rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043wrfifo.r51:	libmf$(LIBSUFFIX)/ax5043wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5043regs.r51:	../source/ax5043regs.c ../source/ax8052f143.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051comminit.r51:	libmf$(LIBSUFFIX)/ax5051comminit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051commslpexit.r51:	libmf$(LIBSUFFIX)/ax5051commslpexit.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051reset.r51:	libmf$(LIBSUFFIX)/ax5051reset.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051deepsleep.r51:	libmf$(LIBSUFFIX)/ax5051deepsleep.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkena.r51:	../source/ax5051rclkena.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rclkdis.r51:	../source/ax5051rclkdis.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051rdfifo.r51:	libmf$(LIBSUFFIX)/ax5051rdfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051wrfifo.r51:	libmf$(LIBSUFFIX)/ax5051wrfifo.c ../source/radiodefs.h ../source/libmfradio.h ../source/libmftypes.h ../source/ax8052.h ../source/ax8052f131.h ../source/ax8052f142.h ../source/ax8052f143.h ../source/ax8052f151.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax5051regs.r51:	../source/ax5051regs.c ../source/ax8052f151.h ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/ax8052regs.r51:	../source/ax8052regs.c ../source/ax8052.h ../source/axcompiler.h | libmf$(LIBSUFFIX)
libmf$(LIBSUFFIX)/radioregs.r51:	radioregs.s51 | libmf$(LIBSUFFIX)

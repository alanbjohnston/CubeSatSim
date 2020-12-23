all: DEBUG_BEHAVIOR=
all: libax5043.a
all: radioafsk 
all: telem

debug: DEBUG_BEHAVIOR = -DDEBUG_LOGGING
debug: libax5043.a
debug: radioafsk
debug: telem

rebuild: clean
rebuild: all

lib: libax5043.a

clean:
	rm -f radiochat	
	rm -f radiopiglatin
	rm -f testax5043rx
	rm -f testax5043tx
	rm -f testax50432freq
	rm -f testax5043init
	rm -f radioafsk
	rm -f testafsktx
	rm -f libax5043.a
	rm -f */*.o
	rm -f */*/*.o
	rm -rf ax5043/doc/html
	rm -rf ax5043/doc/latex
	rm -f telem

docs:
	mkdir -p ax5043/doc; cd ax5043; doxygen Doxyfile
	cd ax5043/doc/latex && make && cd ../.. && cp doc/latex/refman.pdf doc/TransceiverFramework.pdf

libax5043.a: ax5043/crc/crc.o
libax5043.a: ax5043/ax5043support/ax5043tx.o
libax5043.a: ax5043/ax5043support/ax5043init.o
libax5043.a: ax5043/ax5043support/ax5043rx.o
libax5043.a: ax5043/axradio/axradiorx.o
libax5043.a: ax5043/axradio/axradiomode.o
libax5043.a: ax5043/axradio/axradiotx.o
libax5043.a: ax5043/axradio/axradioinit.o
libax5043.a: ax5043/generated/configrx.o
libax5043.a: ax5043/generated/configtx.o
libax5043.a: ax5043/generated/config.o
libax5043.a: ax5043/generated/configcommon.o
libax5043.a: ax5043/spi/ax5043spi.o
	ar rcsv libax5043.a ax5043/generated/configcommon.o ax5043/generated/configtx.o ax5043/generated/configrx.o ax5043/generated/config.o ax5043/axradio/axradioinit.o ax5043/axradio/axradiomode.o ax5043/axradio/axradiotx.o ax5043/axradio/axradiorx.o ax5043/crc/crc.o ax5043/spi/ax5043spi.o ax5043/ax5043support/ax5043tx.o ax5043/ax5043support/ax5043init.o ax5043/ax5043support/ax5043rx.o

radiochat: libax5043.a
radiochat: chat/chat_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o radiochat -pthread -L./ chat/chat_main.o -lwiringPi -lax5043

radiopiglatin: libax5043.a
radiopiglatin: piglatin/piglatin_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o radiopiglatin -Wall -Wextra -L./ piglatin/piglatin_main.o -lwiringPi -lax5043

testax5043tx: libax5043.a
testax5043tx: transmit/transmit_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o testax5043tx -Wall -Wextra -L./ transmit/transmit_main.o -lwiringPi -lax5043

testax5043rx: libax5043.a
testax5043rx: receive/receive_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o testax5043rx -Wall -Wextra -L./ receive/receive_main.o -lwiringPi -lax5043

testax5043init: libax5043.a
testax5043init: init/init_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o testax5043init -Wall -Wextra -L./ init/init_main.o -lwiringPi -lax5043 

testax50432freq: libax5043.a
testax50432freq: transmit2freq/transmit2freq_main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o testax50432freq -Wall -Wextra -L./ transmit2freq/transmit2freq_main.o -lwiringPi -lax5043 

testafsktx: libax5043.a
testafsktx: afsktx/ax25.o
testafsktx: afsktx/ax5043.o
testafsktx: afsktx/main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o testafsktx -Wall -Wextra -L./ afsktx/ax25.o afsktx/ax5043.o afsktx/main.o -lwiringPi -lax5043 

radioafsk: libax5043.a
radioafsk: afsk/ax25.o
radioafsk: afsk/ax5043.o
radioafsk: afsk/main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o radioafsk -Wall -Wextra -L./ afsk/ax25.o afsk/ax5043.o afsk/main.o -lwiringPi -lax5043 -lm

telem: afsk/telem.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o telem -Wall -Wextra -L./ afsk/telem.o -lwiringPi 

ax5043/generated/configcommon.o: ax5043/generated/configcommon.c
ax5043/generated/configcommon.o: ax5043/generated/configrx.h
ax5043/generated/configcommon.o: ax5043/generated/configtx.h
ax5043/generated/configcommon.o: ax5043/axradio/axradioinit.h
ax5043/generated/configcommon.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c configcommon.c

ax5043/generated/configrx.o: ax5043/generated/configrx.c
ax5043/generated/configrx.o: ax5043/generated/configrx.h
ax5043/generated/configrx.o: ax5043/axradio/axradioinit.h
ax5043/generated/configrx.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c configrx.c

ax5043/generated/configtx.o: ax5043/generated/configtx.c
ax5043/generated/configtx.o: ax5043/generated/configtx.h
ax5043/generated/configtx.o: ax5043/axradio/axradioinit.h
ax5043/generated/configtx.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c configtx.c

ax5043/generated/config.o: ax5043/generated/config.c
ax5043/generated/config.o: ax5043/generated/config.h
ax5043/generated/config.o: ax5043/axradio/axradioinit.h
ax5043/generated/config.o: ax5043/axradio/axradioinit_p.h
ax5043/generated/config.o: ax5043/spi/ax5043spi.h
ax5043/generated/config.o: ax5043/spi/ax5043spi_p.h
ax5043/generated/config.o: ax5043/crc/crc.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c config.c

ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi.c
ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi.h
ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/spi; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c ax5043spi.c

ax5043/ax5043support/ax5043init.o: ax5043/ax5043support/ax5043init.c
ax5043/ax5043support/ax5043init.o: ax5043/ax5043support/ax5043init.h
ax5043/ax5043support/ax5043init.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043init.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043init.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043init.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c ax5043init.c

ax5043/ax5043support/ax5043rx.o: ax5043/ax5043support/ax5043rx.c
ax5043/ax5043support/ax5043rx.o: ax5043/ax5043support/ax5043rx.h
ax5043/ax5043support/ax5043rx.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043rx.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043rx.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043rx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c ax5043rx.c

ax5043/ax5043support/ax5043tx.o: ax5043/ax5043support/ax5043tx.c
ax5043/ax5043support/ax5043tx.o: ax5043/ax5043support/ax5043tx.h
ax5043/ax5043support/ax5043tx.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043tx.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043tx.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043tx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c ax5043tx.c

ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit.c
ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradioinit.o: ax5043/ax5043support/ax5043init.h
ax5043/axradio/axradioinit.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradioinit.o: ax5043/spi/ax5043spi_p.h
ax5043/axradio/axradioinit.o: ax5043/generated/config.h
ax5043/axradio/axradioinit.o: ax5043/crc/crc.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c axradioinit.c

ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode.c
ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode_p.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradiomode.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradiomode.o: ax5043/spi/ax5043spi_p.h
ax5043/axradio/axradiomode.o: ax5043/generated/config.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c axradiomode.c

ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx.c
ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx_p.h
ax5043/axradio/axradiorx.o: ax5043/ax5043support/ax5043rx.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradiorx.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradiorx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c axradiorx.c

ax5043/axradio/axradiotx.o: ax5043/axradio/axradiotx.c
ax5043/axradio/axradiotx.o: ax5043/axradio/axradiotx.h
ax5043/axradio/axradiotx.o: ax5043/axradio/axradiotx_p.h
ax5043/axradio/axradiotx.o: ax5043/ax5043support/ax5043tx.h
ax5043/axradio/axradiotx.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradiotx.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradiotx.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradiotx.o: ax5043/spi/ax5043spi_p.h
ax5043/axradio/axradiotx.o: ax5043/generated/config.h
ax5043/axradio/axradiotx.o: ax5043/axradio/axradiomode.h
ax5043/axradio/axradiotx.o: ax5043/axradio/axradiomode_p.h
ax5043/axradio/axradiotx.o: ax5043/crc/crc.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c axradiotx.c

ax5043/crc/crc.o: ax5043/crc/crc.c
ax5043/crc/crc.o: ax5043/crc/crc.h
	cd ax5043/crc; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c crc.c

chat/chat_main.o: chat/chat_main.c
chat/chat_main.o: ax5043/spi/ax5043spi.h
chat/chat_main.o: ax5043/spi/ax5043spi_p.h
chat/chat_main.o: ax5043/axradio/axradioinit.h
chat/chat_main.o: ax5043/axradio/axradioinit_p.h
chat/chat_main.o: ax5043/axradio/axradiomode.h
chat/chat_main.o: ax5043/axradio/axradiomode_p.h
chat/chat_main.o: ax5043/axradio/axradiorx.h
chat/chat_main.o: ax5043/axradio/axradiorx_p.h
chat/chat_main.o: ax5043/axradio/axradiotx.h
chat/chat_main.o: ax5043/axradio/axradiotx_p.h
chat/chat_main.o: ax5043/generated/configtx.h
	cd chat; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I../ax5043 -c chat_main.c; cd ..

afsk/ax25.o: afsk/ax25.c
afsk/ax25.o: afsk/ax25.h
afsk/ax25.o: afsk/ax5043.h
afsk/ax25.o: afsk/status.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c ax25.c; cd ..

afsk/ax5043.o: afsk/ax5043.c
afsk/ax5043.o: afsk/ax25.h
afsk/ax5043.o: afsk/ax5043.h
afsk/ax5043.o: afsk/status.h
afsk/ax5043.o: afsk/utils.h
afsk/ax5043.o: afsk/main.c
afsk/ax5043.o: ax5043/spi/ax5043spi.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c ax5043.c; cd ..

afsk/main.o: afsk/main.c
afsk/main.o: afsk/status.h
afsk/main.o: afsk/ax5043.h
afsk/main.o: afsk/ax25.h
afsk/main.o: ax5043/spi/ax5043spi.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c main.c; cd ..

afsk/telem.o: afsk/telem.c
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c telem.c; cd ..

cw/cw_main.o: cw/cw_main.c
cw/cw_main.o: ax5043/spi/ax5043spi.h
cw/cw_main.o: ax5043/spi/ax5043spi_p.h
cw/cw_main.o: ax5043/axradio/axradioinit.h
cw/cw_main.o: ax5043/axradio/axradioinit_p.h
cw/cw_main.o: ax5043/axradio/axradiomode.h
cw/cw_main.o: ax5043/axradio/axradiomode_p.h
cw/cw_main.o: ax5043/axradio/axradiorx.h
cw/cw_main.o: ax5043/axradio/axradiorx_p.h
cw/cw_main.o: ax5043/axradio/axradiotx.h
cw/cw_main.o: ax5043/axradio/axradiotx_p.h
cw/cw_main.o: ax5043/generated/configtx.h
	cd cw; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I../ax5043 -c cw_main.c; cd ..

piglatin/piglatin_main.o: piglatin/piglatin_main.c
piglatin/piglatin_main.o: ax5043/spi/ax5043spi.h
piglatin/piglatin_main.o: ax5043/spi/ax5043spi_p.h
piglatin/piglatin_main.o: ax5043/axradio/axradioinit.h
piglatin/piglatin_main.o: ax5043/axradio/axradioinit_p.h
piglatin/piglatin_main.o: ax5043/axradio/axradiomode.h
piglatin/piglatin_main.o: ax5043/axradio/axradiomode_p.h
piglatin/piglatin_main.o: ax5043/axradio/axradiorx.h
piglatin/piglatin_main.o: ax5043/axradio/axradiorx_p.h
piglatin/piglatin_main.o: ax5043/axradio/axradiotx.h
piglatin/piglatin_main.o: ax5043/axradio/axradiotx_p.h
piglatin/piglatin_main.o: ax5043/generated/configtx.h
	cd piglatin; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c piglatin_main.c; cd ..

receive/receive_main.o: receive/receive_main.c
receive/receive_main.o: ax5043/axradio/axradioinit.h
receive/receive_main.o: ax5043/axradio/axradioinit_p.h
receive/receive_main.o: ax5043/spi/ax5043spi.h
receive/receive_main.o: ax5043/spi/ax5043spi_p.h
receive/receive_main.o: ax5043/axradio/axradiomode.h
receive/receive_main.o: ax5043/axradio/axradiomode_p.h
receive/receive_main.o: ax5043/axradio/axradiorx.h
receive/receive_main.o: ax5043/axradio/axradiorx_p.h
receive/receive_main.o: ax5043/generated/configrx.h
	cd receive; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c receive_main.c; cd ..

transmit/transmit_main.o: transmit/transmit_main.c
transmit/transmit_main.o: ax5043/axradio/axradioinit.h
transmit/transmit_main.o: ax5043/axradio/axradioinit_p.h
transmit/transmit_main.o: ax5043/spi/ax5043spi.h
transmit/transmit_main.o: ax5043/spi/ax5043spi_p.h
transmit/transmit_main.o: ax5043/axradio/axradiomode.h
transmit/transmit_main.o: ax5043/axradio/axradiomode_p.h
transmit/transmit_main.o: ax5043/axradio/axradiotx.h
transmit/transmit_main.o: ax5043/axradio/axradiotx_p.h
transmit/transmit_main.o: ax5043/generated/configtx.h
	cd transmit; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c transmit_main.c; cd ..

transmit2freq/transmit2freq_main.o: transmit2freq/transmit2freq_main.c
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradioinit.h
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradioinit_p.h
transmit2freq/transmit2freq_main.o: ax5043/spi/ax5043spi.h
transmit2freq/transmit2freq_main.o: ax5043/spi/ax5043spi_p.h
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradiomode.h
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradiomode_p.h
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradiotx.h
transmit2freq/transmit2freq_main.o: ax5043/axradio/axradiotx_p.h
transmit2freq/transmit2freq_main.o: ax5043/generated/configtx.h
	cd transmit2freq; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c transmit2freq_main.c; cd ..

init/init_main.o: init/init_main.c
init/init_main.o: ax5043/axradio/axradioinit.h
init/init_main.o: ax5043/axradio/axradioinit_p.h
init/init_main.o: ax5043/spi/ax5043spi.h
init/init_main.o: ax5043/spi/ax5043spi_p.h
	cd init; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c init_main.c; cd ..

afsktx/ax25.o: afsktx/ax25.c
afsktx/ax25.o: afsktx/ax25.h
afsktx/ax25.o: afsktx/ax5043.h
afsktx/ax25.o: afsktx/status.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c ax25.c; cd ..

afsktx/ax5043.o: afsktx/ax5043.c
afsktx/ax5043.o: afsktx/ax25.h
afsktx/ax5043.o: afsktx/ax5043.h
afsktx/ax5043.o: afsktx/status.h
afsktx/ax5043.o: afsktx/utils.h
afsktx/ax5043.o: ax5043/spi/ax5043spi.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c ax5043.c; cd ..

afsktx/main.o: afsktx/main.c
afsktx/main.o: afsktx/status.h
afsktx/main.o: afsktx/ax5043.h
afsktx/main.o: afsktx/ax25.h
afsktx/main.o: ax5043/spi/ax5043spi.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c main.c; cd ..

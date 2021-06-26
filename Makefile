all: DEBUG_BEHAVIOR=
all: libax5043.a
all: cubesatsim 
all: telem

debug: DEBUG_BEHAVIOR = -DDEBUG_LOGGING
debug: libax5043.a
debug: cubesatsim
debug: telem

rebuild: clean
rebuild: all

lib: libax5043.a

clean:

	rm -f cubesatsim
	rm -f libax5043.a
	rm -f */*.o
	rm -f */*/*.o
	rm -f telem

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

cubesatsim: main.h
cubesatsim: libax5043.a
cubesatsim: afsk/ax25.o
cubesatsim: afsk/ax5043.o
cubesatsim: TelemEncoding.o
cubesatsim: main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o cubesatsim -Wall -Wextra -L./ afsk/ax25.o afsk/ax5043.o TelemEncoding.o main.o -lwiringPi -lax5043 -lm

telem: telem.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o telem -Wall -Wextra -L./ telem.o -lwiringPi 

TelemEncoding.o: TelemEncoding.c
TelemEncoding.o: TelemEncoding.h
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -Wall -Wextra -c TelemEncoding.c

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
afsk/ax5043.o: ax5043/spi/ax5043spi.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c ax5043.c; cd ..

main.o: main.h
main.o: main.c
main.o: afsk/status.h
main.o: afsk/ax5043.h
main.o: afsk/ax25.h
main.o: ax5043/spi/ax5043spi.h
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c main.c; 	

telem.o: telem.c
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -c telem.c; 

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

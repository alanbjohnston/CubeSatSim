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
	rm -f radioafsk
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

radioafsk: libax5043.a
radioafsk: afsk/ax25.o
radioafsk: afsk/ax5043.o
radioafsk: afsk/main.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o radioafsk -pedantic -Wall -Wextra -L./ afsk/ax25.o afsk/ax5043.o afsk/main.o -lwiringPi -lax5043 -lm

telem: afsk/telem.o
	gcc -std=gnu99 $(DEBUG_BEHAVIOR) -o telem -pedantic -Wall -Wextra -L./ afsk/telem.o -lwiringPi 

ax5043/generated/configcommon.o: ax5043/generated/configcommon.c
ax5043/generated/configcommon.o: ax5043/generated/configrx.h
ax5043/generated/configcommon.o: ax5043/generated/configtx.h
ax5043/generated/configcommon.o: ax5043/axradio/axradioinit.h
ax5043/generated/configcommon.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c configcommon.c

ax5043/generated/configrx.o: ax5043/generated/configrx.c
ax5043/generated/configrx.o: ax5043/generated/configrx.h
ax5043/generated/configrx.o: ax5043/axradio/axradioinit.h
ax5043/generated/configrx.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c configrx.c

ax5043/generated/configtx.o: ax5043/generated/configtx.c
ax5043/generated/configtx.o: ax5043/generated/configtx.h
ax5043/generated/configtx.o: ax5043/axradio/axradioinit.h
ax5043/generated/configtx.o: ax5043/axradio/axradioinit_p.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c configtx.c

ax5043/generated/config.o: ax5043/generated/config.c
ax5043/generated/config.o: ax5043/generated/config.h
ax5043/generated/config.o: ax5043/axradio/axradioinit.h
ax5043/generated/config.o: ax5043/axradio/axradioinit_p.h
ax5043/generated/config.o: ax5043/spi/ax5043spi.h
ax5043/generated/config.o: ax5043/spi/ax5043spi_p.h
ax5043/generated/config.o: ax5043/crc/crc.h
	cd ax5043/generated; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c config.c

ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi.c
ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi.h
ax5043/spi/ax5043spi.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/spi; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c ax5043spi.c

ax5043/ax5043support/ax5043init.o: ax5043/ax5043support/ax5043init.c
ax5043/ax5043support/ax5043init.o: ax5043/ax5043support/ax5043init.h
ax5043/ax5043support/ax5043init.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043init.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043init.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043init.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c ax5043init.c

ax5043/ax5043support/ax5043rx.o: ax5043/ax5043support/ax5043rx.c
ax5043/ax5043support/ax5043rx.o: ax5043/ax5043support/ax5043rx.h
ax5043/ax5043support/ax5043rx.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043rx.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043rx.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043rx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c ax5043rx.c

ax5043/ax5043support/ax5043tx.o: ax5043/ax5043support/ax5043tx.c
ax5043/ax5043support/ax5043tx.o: ax5043/ax5043support/ax5043tx.h
ax5043/ax5043support/ax5043tx.o: ax5043/axradio/axradioinit.h
ax5043/ax5043support/ax5043tx.o: ax5043/axradio/axradioinit_p.h
ax5043/ax5043support/ax5043tx.o: ax5043/spi/ax5043spi.h
ax5043/ax5043support/ax5043tx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/ax5043support; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c ax5043tx.c

ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit.c
ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradioinit.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradioinit.o: ax5043/ax5043support/ax5043init.h
ax5043/axradio/axradioinit.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradioinit.o: ax5043/spi/ax5043spi_p.h
ax5043/axradio/axradioinit.o: ax5043/generated/config.h
ax5043/axradio/axradioinit.o: ax5043/crc/crc.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c axradioinit.c

ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode.c
ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradiomode_p.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradiomode.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradiomode.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradiomode.o: ax5043/spi/ax5043spi_p.h
ax5043/axradio/axradiomode.o: ax5043/generated/config.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c axradiomode.c

ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx.c
ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradiorx_p.h
ax5043/axradio/axradiorx.o: ax5043/ax5043support/ax5043rx.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradioinit.h
ax5043/axradio/axradiorx.o: ax5043/axradio/axradioinit_p.h
ax5043/axradio/axradiorx.o: ax5043/spi/ax5043spi.h
ax5043/axradio/axradiorx.o: ax5043/spi/ax5043spi_p.h
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c axradiorx.c

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
	cd ax5043/axradio; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c axradiotx.c

ax5043/crc/crc.o: ax5043/crc/crc.c
ax5043/crc/crc.o: ax5043/crc/crc.h
	cd ax5043/crc; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -pedantic -Wall -Wextra -c crc.c

afsk/ax25.o: afsk/ax25.c
afsk/ax25.o: afsk/ax25.h
afsk/ax25.o: afsk/ax5043.h
afsk/ax25.o: afsk/status.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c ax25.c; cd ..

afsk/ax5043.o: afsk/ax5043.c
afsk/ax5043.o: afsk/ax25.h
afsk/ax5043.o: afsk/ax5043.h
afsk/ax5043.o: afsk/status.h
afsk/ax5043.o: afsk/utils.h
afsk/ax5043.o: afsk/main.c
afsk/ax5043.o: ax5043/spi/ax5043spi.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c ax5043.c; cd ..

afsk/main.o: afsk/main.c
afsk/main.o: afsk/status.h
afsk/main.o: afsk/ax5043.h
afsk/main.o: afsk/ax25.h
afsk/main.o: ax5043/spi/ax5043spi.h
afsk/main.o: afsk/Adafruit_INA219.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c main.c; cd ..

afsk/telem.o: afsk/telem.c
afsk/telem.o: afsk/Adafruit_INA219.h
	cd afsk; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c telem.c; cd ..

afsktx/ax25.o: afsktx/ax25.c
afsktx/ax25.o: afsktx/ax25.h
afsktx/ax25.o: afsktx/ax5043.h
afsktx/ax25.o: afsktx/status.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c ax25.c; cd ..

afsktx/ax5043.o: afsktx/ax5043.c
afsktx/ax5043.o: afsktx/ax25.h
afsktx/ax5043.o: afsktx/ax5043.h
afsktx/ax5043.o: afsktx/status.h
afsktx/ax5043.o: afsktx/utils.h
afsktx/ax5043.o: ax5043/spi/ax5043spi.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c ax5043.c; cd ..

afsktx/main.o: afsktx/main.c
afsktx/main.o: afsktx/status.h
afsktx/main.o: afsktx/ax5043.h
afsktx/main.o: afsktx/ax25.h
afsktx/main.o: ax5043/spi/ax5043spi.h
	cd afsktx; gcc -std=gnu99 $(DEBUG_BEHAVIOR) -I ../ax5043 -pedantic -Wconversion -Wall -Wextra -c main.c; cd ..

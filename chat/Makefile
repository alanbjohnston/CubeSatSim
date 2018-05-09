all: radiochat

rebuild: clean
rebuild: all

clean:
	rm -f radiochat
	rm -f *.o
	rm -f */*.o
	
radiochat: spi/ax5043spi.o
radiochat: chat/chat_main.o
radiochat: crc/crc.o
	gcc -o radiochat -pthread -Llibs/. spi/ax5043spi.o chat/chat_main.o crc/crc.o -lwiringPi -lax5043

spi/ax5043spi.o: spi/ax5043spi.c
spi/ax5043spi.o: spi/ax5043spi.h
	cd spi; gcc -Wall -Wextra -c ax5043spi.c; cd ..

chat/chat_main.o: chat/chat_main.c
chat/chat_main.o: ax5043/ax5043init.h
chat/chat_main.o: spi/ax5043spi.h
chat/chat_main.o: ax5043/ax5043mode.h
chat/chat_main.o: ax5043/ax5043rx.h
chat/chat_main.o: generated/configrx.h
	cd chat; gcc -Wall -Wextra -c chat_main.c; cd ..

crc/crc.o: crc/crc.c
crc/crc.o: crc/crc.h
	cd crc; gcc -Wall -Wextra -c crc.c; cd ..


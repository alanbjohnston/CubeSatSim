all: radiochat

rebuild: clean
rebuild: all

clean:
	rm -f radiochat
	rm -f *.o
	rm -f */*.o
	
radiochat: chat/chat_main.o
	gcc -Wall -Wextra -o radiochat -pthread -Llibs/. chat/chat_main.o -lwiringPi -lax5043

chat/chat_main.o: chat/chat_main.c
chat/chat_main.o: spi/ax5043spi_p.h
chat/chat_main.o: ax5043/ax5043mode_p.h
chat/chat_main.o: ax5043/ax5043rx_p.h
chat/chat_main.o: ax5043/ax5043tx_p.h
chat/chat_main.o: generated/configtx.h
	cd chat; gcc -Wall -Wextra -c chat_main.c; cd ..


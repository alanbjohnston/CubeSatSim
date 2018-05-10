all: probeTxRx

rebuild: clean
rebuild: all

clean:
	rm -f probeTxRx
	rm -f probeTxRx.o

probeTxRx: probeTxRx.o
	gcc -Wall -Wextra -o probeTxRx -Llibs/. probeTxRx.o -lwiringPi -lax5043

probeTxRx.o: probeTxRx.c
probeTxRx.o: spi/ax5043spi_p.h
	gcc -Wall -Wextra -c probeTxRx.c

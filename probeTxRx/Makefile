all: probeTxRx

rebuild: clean
rebuild: all

clean:
	rm -f probeTxRx
	rm -f probeTxRx.o
	rm -f spi/ax5043spi.o

probeTxRx: probeTxRx.o
probeTxRx: spi/ax5043spi.o
	gcc -Wall -Wextra -o probeTxRx -lwiringPi probeTxRx.o spi/ax5043spi.o

probeTxRx.o: probeTxRx.c
probeTxRx.o: spi/ax5043spi.h
	gcc -Wall -Wextra -c probeTxRx.c

spi/ax5043spi.o: spi/ax5043spi.c
spi/ax5043spi.o: spi/ax5043spi.h
	gcc -Wall -Wextra -c -o spi/ax5043spi.o spi/ax5043spi.c

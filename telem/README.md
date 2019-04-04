To compile:

gcc -pedantic -Wall -Wextra -c readall.c
gcc -o readall -pedantic -Wall -Wextra -L./ readall.o -lwiringPi

Then to run:

./readll

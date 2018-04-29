#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "spi/ax5043spi.h"

#define AX5043_PWRMODE            (0x002)
#define AX5043_PWRSTATE_POWERDOWN (0x000)
#define AX5043_SILICONREVISION    (0x000)
#define AX5043_SCRATCH            (0x001)
#define SILICONREV1               (0x51)

int main() {
	setSpiChannel(SPI_CHANNEL);
        setSpiSpeed(SPI_SPEED);
        initializeSpi();

	printf("INFO: Resetting AX5043\n");

	uint8_t i;

	// Initialize Interface
	// Reset Device
	ax5043WriteReg(AX5043_PWRMODE, 0x80);
	ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);

	// Wait some time for regulator startup
	usleep(10000);

	// Check Scratch
	i = ax5043ReadReg(AX5043_SILICONREVISION);
	i = ax5043ReadReg(AX5043_SILICONREVISION);

	if (i != SILICONREV1) {
		printf("ERROR: Unexpected hardware revision: %d\n", (int)i);
		exit(EXIT_FAILURE);
	}

	ax5043WriteReg(AX5043_SCRATCH, 0x55);

	if (ax5043ReadReg(AX5043_SCRATCH) != 0x55) {
		printf("ERROR: Unable to communicate with the AX5043\n");
		exit(EXIT_FAILURE);
	}

	ax5043WriteReg(AX5043_SCRATCH, 0xAA);
	if (ax5043ReadReg(AX5043_SCRATCH) != 0xAA) {
		printf("ERROR: Unable to communicate with the AX5043\n");
		exit(EXIT_FAILURE);
	}

	printf("INFO: Successfully initialized the AX5043\n");
	exit(EXIT_SUCCESS);
}

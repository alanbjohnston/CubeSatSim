// Copyright (c) 2018 Brandenburg Tech, LLC

#include "ax5043spi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#include "dummyspi.h"
//#warning "For production builds, must not include dummyspi.h"
#include <wiringPiSPI.h>
#include <wiringPi.h>

int spiChannel = -1;
int spiSpeed = -1;

void setSpiChannel(int newSpiChannel) {
	spiChannel = newSpiChannel;
}

void setSpiSpeed(int newSpiSpeed) {
	spiSpeed = newSpiSpeed;
}

void initializeSpi() {
	printf("INFO: Initializing SPI\n");

	if (spiChannel < 0) {
		fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
		exit(EXIT_FAILURE);
	}
	if (spiSpeed < 0) {
		fprintf(stderr, "ERROR: invalid SPI speed %d\n", spiSpeed);
		exit(EXIT_FAILURE);
	}


	int fd;

	wiringPiSetup();

	fd = wiringPiSPISetup(spiChannel, spiSpeed);

	if (fd < 0) {
		fprintf(stderr, "ERROR: Cannot open SPI bus with error %d, %s\n",
				errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("INFO: Finished initializing SPI\n");
}

void ax5043WriteReg(uint16_t reg, uint8_t val) {
	uint8_t buf[3];
	int result;

	if (spiChannel < 0) {
		fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
		exit(EXIT_FAILURE);
	}

	buf[0] = (unsigned char)(0x00f0 | ((reg & 0xf00) >> 8));
	buf[1] = (reg & 0xff);
	buf[2] = val & 0xff;

	result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
	if (result < 0) {
		fprintf(stderr, "Failed to write the register with result %d and error %s\n",
				result, strerror(result));
		exit(EXIT_FAILURE);
	}
}

uint8_t ax5043ReadReg(uint16_t reg) {
	uint8_t buf[3];
	int result;

	if (spiChannel < 0) {
		fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
		exit(EXIT_FAILURE);
	}

	buf[0] = (unsigned char)(0x0070 | ((reg & 0xf00) >> 8));
	buf[1] = (reg & 0xff);
	buf[2] = 0x0000;

	result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
	if (result < 0) {
		fprintf(stderr, "Failed to read register with result = %d and error %s\n",
				result, strerror(errno));
		exit(EXIT_FAILURE);
	}

	//printf("DEBUG: read value: %d\n", (int)buf[2]);
	return (buf[2] & 0xff);
}

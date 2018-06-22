// Copyright (c) 2018 Brandenburg Tech, LLC
// All right reserved.
//
// THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
// AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ax5043spi.h"

//#include <process.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "dummyspi.h"
//#warning "For production builds, must not include dummyspi.h"
#include <wiringPiSPI.h>
#include <wiringPi.h>

#define MAX_SPI_WRITE_SIZE (512)

int spiChannel = -1;
int spiSpeed = -1;

void setSpiChannel(int newSpiChannel) {
    spiChannel = newSpiChannel;
}

void setSpiSpeed(int newSpiSpeed) {
    spiSpeed = newSpiSpeed;
}

void initializeSpi() {
    //printf("INFO: Initializing SPI\n");

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

    //printf("INFO: Finished initializing SPI\n");
}

void ax5043WriteReg(uint16_t reg, uint8_t val) {
    uint8_t buf[3];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = val & 0xff;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg2(uint16_t reg, uint16_t val) {
    uint8_t buf[4];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 8) & 0xff;
    buf[3] = val & 0xff;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg3(uint16_t reg, uint32_t val) {
    uint8_t buf[5];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 16)& 0xff;
    buf[3] = (val >> 8) & 0xff;
    buf[4] = val & 0xff;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg4(uint16_t reg, uint32_t val) {
    uint8_t buf[6];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 24) & 0xff;
    buf[3] = (val >> 16) & 0xff;
    buf[4] = (val >> 8) & 0xff;
    buf[5] = val & 0xff;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteRegN(uint16_t reg, const uint8_t *in, uint32_t len) {
    uint8_t buf[MAX_SPI_WRITE_SIZE + 2];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }
    if (len > MAX_SPI_WRITE_SIZE) {
        fprintf(stderr,
                "ERROR: attempting to write too much data to SPI channel (max of %d): %d\n",
                MAX_SPI_WRITE_SIZE, len);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    for (uint32_t i = 0; i < len; ++i) {
        buf[i + 2] = *(in + i);
    }

    result = wiringPiSPIDataRW(spiChannel, buf, len + 2);
    if (result < 0) {
        fprintf(stderr,
                "Failed to write the register with result %d and error %s\n",
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

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (buf[2]);
}

uint16_t ax5043ReadReg2(uint16_t reg) {
    uint8_t buf[4];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (buf[3]) | (buf[2] << 8);
}

uint32_t ax5043ReadReg3(uint16_t reg) {
    uint8_t buf[5];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;
    buf[4] = 0x0000;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (buf[4]) | (buf[3] << 8) | (buf[2] << 16);
}

uint32_t ax5043ReadReg4(uint16_t reg) {
    uint8_t buf[6];
    int result;

    if (spiChannel < 0) {
        fprintf(stderr, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;
    buf[4] = 0x0000;
    buf[5] = 0x0000;

    result = wiringPiSPIDataRW(spiChannel, buf, sizeof(buf));
    if (result < 0) {
        fprintf(stderr,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (buf[5]) | (buf[4] << 8) | (buf[3] << 16) | (buf[2] << 24);
}

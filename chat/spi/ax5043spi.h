// Copyright (c) 2018 Brandenburg Tech, LLC

#ifndef AX5043SPI_H_
#define AX5043SPI_H_

#include <stdint.h>

#define SPI_CHANNEL (0)
#define SPI_SPEED (32000000)

void setSpiChannel(int newSpiChannel);
void setSpiSpeed(int newSpiSpeed);
void initializeSpi();
void ax5043WriteReg(uint16_t reg, uint8_t val);
uint8_t ax5043ReadReg(uint16_t reg);

#endif /* AX5043SPI_H_ */

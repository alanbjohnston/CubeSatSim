/*-------------------------------------------------------------------------
   AX8052F131.h - Register Declarations for the Axsem Microfoot Processor Range

   Copyright (C) 2010, 2011, 2012, 2016 Axsem AG
     Author: Thomas Sailer, thomas.sailer@axsem.com

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/


#ifndef AX8052F131_H
#define AX8052F131_H

#include <ax8052.h>

/* Radio Registers, X Address Space */

SFRX(AX5031_CRCINIT0,              0x4017)   /* CRC Initial Value 0 */
SFRX(AX5031_CRCINIT1,              0x4016)   /* CRC Initial Value 1 */
SFRX(AX5031_CRCINIT2,              0x4015)   /* CRC Initial Value 2 */
SFRX(AX5031_CRCINIT3,              0x4014)   /* CRC Initial Value 3 */
SFRX(AX5031_ENCODING,              0x4011)   /* Encoding */
SFRX(AX5031_FEC,                   0x4018)   /* Forward Error Correction */
SFRX(AX5031_FIFOCONTROL,           0x4004)   /* FIFO Control */
SFRX(AX5031_FIFOCONTROL2,          0x4037)   /* FIFO Control 2 */
SFRX(AX5031_FIFOCOUNT,             0x4035)   /* FIFO Count */
SFRX(AX5031_FIFODATA,              0x4005)   /* FIFO Data */
SFRX(AX5031_FIFOTHRESH,            0x4036)   /* FIFO Threshold */
SFRX(AX5031_FOURFSK,               0x4050)   /* 4-FSK Control */
SFRX(AX5031_FRAMING,               0x4012)   /* Framing Mode */
SFRX(AX5031_FREQA0,                0x4023)   /* Frequency A 0 */
SFRX(AX5031_FREQA1,                0x4022)   /* Frequency A 1 */
SFRX(AX5031_FREQA2,                0x4021)   /* Frequency A 2 */
SFRX(AX5031_FREQA3,                0x4020)   /* Frequency A 3 */
SFRX(AX5031_FREQB0,                0x401F)   /* Frequency B 0 */
SFRX(AX5031_FREQB1,                0x401E)   /* Frequency B 1 */
SFRX(AX5031_FREQB2,                0x401D)   /* Frequency B 2 */
SFRX(AX5031_FREQB3,                0x401C)   /* Frequency B 3 */
SFRX(AX5031_FSKDEV0,               0x4027)   /* FSK Deviation 0 */
SFRX(AX5031_FSKDEV1,               0x4026)   /* FSK Deviation 1 */
SFRX(AX5031_FSKDEV2,               0x4025)   /* FSK Deviation 2 */
SFRX(AX5031_IFMODE,                0x4008)   /* Interface Mode */
SFRX(AX5031_IRQINVERSION,          0x400F)   /* IRQ Inversion */
SFRX(AX5031_IRQMASK,               0x4006)   /* IRQ Mask */
SFRX(AX5031_IRQREQUEST,            0x4007)   /* IRQ Request */
SFRX(AX5031_MODULATION,            0x4010)   /* Modulation */
SFRX(AX5031_MODULATORMISC,         0x4034)   /* Modulator Miscellaneous Control */
SFRX(AX5031_PINCFG1,               0x400C)   /* Pin Configuration 1 */
SFRX(AX5031_PINCFG2,               0x400D)   /* Pin Configuration 2 */
SFRX(AX5031_PINCFG3,               0x400E)   /* Pin Configuration 3 */
SFRX(AX5031_PLLLOOP,               0x402C)   /* PLL Loop Filter */
SFRX(AX5031_PLLRANGING,            0x402D)   /* PLL Autoranging Control */
SFRX(AX5031_PLLRNGCLK,             0x402E)   /* PLL Autoranging Clock */
SFRX(AX5031_PWRMODE,               0x4002)   /* Power Mode */
SFRX(AX5031_SCRATCH,               0x4001)   /* Scratch */
SFRX(AX5031_SILICONREVISION,       0x4000)   /* Silicon Revision */
SFRX(AX5031_TXBITRATEHI,           0x4031)   /* Transmitter Bitrate High */
SFRX(AX5031_TXBITRATELO,           0x4033)   /* Transmitter Bitrate Low */
SFRX(AX5031_TXBITRATEMID,          0x4032)   /* Transmitter Bitrate Middle */
SFRX(AX5031_TXPWR,                 0x4030)   /* Transmit Power */
SFRX(AX5031_VREG,                  0x401B)   /* Voltage Regulator */
SFRX(AX5031_XTALCAP,               0x404F)   /* Crystal Oscillator Load Capacitance */
SFRX(AX5031_XTALOSC,               0x4003)   /* Crystal Oscillator Control */
SFRX(AX5031_XTALOSCCFG,            0x4051)   /* Crystal Oscillator Mode Configuration */

/* Radio Registers, X Address Space, Non-Blocking Version */

SFRX(AX5031_CRCINIT0NB,            0x5017)   /* CRC Initial Value 0, Non-Blocking */
SFRX(AX5031_CRCINIT1NB,            0x5016)   /* CRC Initial Value 1, Non-Blocking */
SFRX(AX5031_CRCINIT2NB,            0x5015)   /* CRC Initial Value 2, Non-Blocking */
SFRX(AX5031_CRCINIT3NB,            0x5014)   /* CRC Initial Value 3, Non-Blocking */
SFRX(AX5031_ENCODINGNB,            0x5011)   /* Encoding, Non-Blocking */
SFRX(AX5031_FECNB,                 0x5018)   /* Forward Error Correction, Non-Blocking */
SFRX(AX5031_FIFOCONTROLNB,         0x5004)   /* FIFO Control, Non-Blocking */
SFRX(AX5031_FIFOCONTROL2NB,        0x5037)   /* FIFO Control 2, Non-Blocking */
SFRX(AX5031_FIFOCOUNTNB,           0x5035)   /* FIFO Count, Non-Blocking */
SFRX(AX5031_FIFODATANB,            0x5005)   /* FIFO Data, Non-Blocking */
SFRX(AX5031_FIFOTHRESHNB,          0x5036)   /* FIFO Threshold, Non-Blocking */
SFRX(AX5031_FOURFSKNB,             0x5050)   /* 4-FSK Control, Non-Blocking */
SFRX(AX5031_FRAMINGNB,             0x5012)   /* Framing Mode, Non-Blocking */
SFRX(AX5031_FREQA0NB,              0x5023)   /* Frequency A 0, Non-Blocking */
SFRX(AX5031_FREQA1NB,              0x5022)   /* Frequency A 1, Non-Blocking */
SFRX(AX5031_FREQA2NB,              0x5021)   /* Frequency A 2, Non-Blocking */
SFRX(AX5031_FREQA3NB,              0x5020)   /* Frequency A 3, Non-Blocking */
SFRX(AX5031_FREQB0NB,              0x501F)   /* Frequency B 0, Non-Blocking */
SFRX(AX5031_FREQB1NB,              0x501E)   /* Frequency B 1, Non-Blocking */
SFRX(AX5031_FREQB2NB,              0x501D)   /* Frequency B 2, Non-Blocking */
SFRX(AX5031_FREQB3NB,              0x501C)   /* Frequency B 3, Non-Blocking */
SFRX(AX5031_FSKDEV0NB,             0x5027)   /* FSK Deviation 0, Non-Blocking */
SFRX(AX5031_FSKDEV1NB,             0x5026)   /* FSK Deviation 1, Non-Blocking */
SFRX(AX5031_FSKDEV2NB,             0x5025)   /* FSK Deviation 2, Non-Blocking */
SFRX(AX5031_IFMODENB,              0x5008)   /* Interface Mode, Non-Blocking */
SFRX(AX5031_IRQINVERSIONNB,        0x500F)   /* IRQ Inversion, Non-Blocking */
SFRX(AX5031_IRQMASKNB,             0x5006)   /* IRQ Mask, Non-Blocking */
SFRX(AX5031_IRQREQUESTNB,          0x5007)   /* IRQ Request, Non-Blocking */
SFRX(AX5031_MODULATIONNB,          0x5010)   /* Modulation, Non-Blocking */
SFRX(AX5031_MODULATORMISCNB,       0x5034)   /* Modulator Miscellaneous Control, Non-Blocking */
SFRX(AX5031_PINCFG1NB,             0x500C)   /* Pin Configuration 1, Non-Blocking */
SFRX(AX5031_PINCFG2NB,             0x500D)   /* Pin Configuration 2, Non-Blocking */
SFRX(AX5031_PINCFG3NB,             0x500E)   /* Pin Configuration 3, Non-Blocking */
SFRX(AX5031_PLLLOOPNB,             0x502C)   /* PLL Loop Filter, Non-Blocking */
SFRX(AX5031_PLLRANGINGNB,          0x502D)   /* PLL Autoranging Control, Non-Blocking */
SFRX(AX5031_PLLRNGCLKNB,           0x502E)   /* PLL Autoranging Clock, Non-Blocking */
SFRX(AX5031_PWRMODENB,             0x5002)   /* Power Mode, Non-Blocking */
SFRX(AX5031_SCRATCHNB,             0x5001)   /* Scratch, Non-Blocking */
SFRX(AX5031_SILICONREVISIONNB,     0x5000)   /* Silicon Revision, Non-Blocking */
SFRX(AX5031_TXBITRATEHINB,         0x5031)   /* Transmitter Bitrate High, Non-Blocking */
SFRX(AX5031_TXBITRATELONB,         0x5033)   /* Transmitter Bitrate Low, Non-Blocking */
SFRX(AX5031_TXBITRATEMIDNB,        0x5032)   /* Transmitter Bitrate Middle, Non-Blocking */
SFRX(AX5031_TXPWRNB,               0x5030)   /* Transmit Power, Non-Blocking */
SFRX(AX5031_VREGNB,                0x501B)   /* Voltage Regulator, Non-Blocking */
SFRX(AX5031_XTALCAPNB,             0x504F)   /* Crystal Oscillator Load Capacitance, Non-Blocking */
SFRX(AX5031_XTALOSCNB,             0x5003)   /* Crystal Oscillator Control, Non-Blocking */
SFRX(AX5031_XTALOSCCFGNB,          0x5051)   /* Crystal Oscillator Mode Configuration, Non-Blocking */

/* Alternate Names */

#define AX5031_MODMISC             AX5031_MODULATORMISC
#define AX5031_TXRATEHI            AX5031_TXBITRATEHI
#define AX5031_TXRATELO            AX5031_TXBITRATELO
#define AX5031_TXRATEMID           AX5031_TXBITRATEMID

#define AX5031_MODMISCNB           AX5031_MODULATORMISCNB
#define AX5031_TXRATEHINB          AX5031_TXBITRATEHINB
#define AX5031_TXRATELONB          AX5031_TXBITRATELONB
#define AX5031_TXRATEMIDNB         AX5031_TXBITRATEMIDNB

#endif /* AX8052F131_H */

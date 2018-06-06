/*-------------------------------------------------------------------------
   AX8052F151.h - Register Declarations for the Axsem Microfoot Processor Range

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


#ifndef AX8052F151_H
#define AX8052F151_H

#include <ax8052.h>

/* Radio Registers, X Address Space */

SFRX(AX5051_ADCMISC,               0x4038)   /* ADC Miscellaneous Control */
SFRX(AX5051_AGCATTACK,             0x403A)   /* AGC Attack Speed */
SFRX(AX5051_AGCCOUNTER,            0x403C)   /* AGC Counter */
SFRX(AX5051_AGCDECAY,              0x403B)   /* AGC Decay Speed */
SFRX(AX5051_AGCTARGET,             0x4039)   /* AGC Target Value */
SFRX(AX5051_AMPLITUDEGAIN,         0x4047)   /* Amplitude Estimator Bandwidth */
SFRX(AX5051_CICDECHI,              0x403E)   /* Decimation Factor High */
SFRX(AX5051_CICDECLO,              0x403F)   /* Decimation Factor Low */
SFRX(AX5051_CICSHIFT,              0x403D)   /* Decimation Filter Attenuation */
SFRX(AX5051_CRCINIT0,              0x4017)   /* CRC Initial Value 0 */
SFRX(AX5051_CRCINIT1,              0x4016)   /* CRC Initial Value 1 */
SFRX(AX5051_CRCINIT2,              0x4015)   /* CRC Initial Value 2 */
SFRX(AX5051_CRCINIT3,              0x4014)   /* CRC Initial Value 3 */
SFRX(AX5051_DATARATEHI,            0x4040)   /* Datarate High */
SFRX(AX5051_DATARATELO,            0x4041)   /* Datarate Low */
SFRX(AX5051_DSPMODE,               0x4009)   /* DSP Mode Interface Control */
SFRX(AX5051_ENCODING,              0x4011)   /* Encoding */
SFRX(AX5051_FEC,                   0x4018)   /* Forward Error Correction */
SFRX(AX5051_FECMEM,                0x406F)   /* Forward Error Correction Memory */
SFRX(AX5051_FECSTATUS,             0x401A)   /* Forward Error Correction Status */
SFRX(AX5051_FECSYNC,               0x4019)   /* Forward Error Correction Sync Threshold */
SFRX(AX5051_FIFOCONTROL,           0x4004)   /* FIFO Control */
SFRX(AX5051_FIFOCONTROL2,          0x4037)   /* FIFO Control 2 */
SFRX(AX5051_FIFOCOUNT,             0x4035)   /* FIFO Count */
SFRX(AX5051_FIFODATA,              0x4005)   /* FIFO Data */
SFRX(AX5051_FIFOTHRESH,            0x4036)   /* FIFO Threshold */
SFRX(AX5051_FRAMING,               0x4012)   /* Framing Mode */
SFRX(AX5051_FREQ0,                 0x4023)   /* Frequency 0 */
SFRX(AX5051_FREQ1,                 0x4022)   /* Frequency 1 */
SFRX(AX5051_FREQ2,                 0x4021)   /* Frequency 2 */
SFRX(AX5051_FREQ3,                 0x4020)   /* Frequency 3 */
SFRX(AX5051_FREQA0,                0x4023)   /* Frequency 0 */
SFRX(AX5051_FREQA1,                0x4022)   /* Frequency 1 */
SFRX(AX5051_FREQA2,                0x4021)   /* Frequency 2 */
SFRX(AX5051_FREQA3,                0x4020)   /* Frequency 3 */
SFRX(AX5051_FREQUENCYGAIN,         0x4045)   /* Frequency Estimator Bandwidth */
SFRX(AX5051_FREQUENCYGAIN2,        0x4046)   /* Frequency Estimator Bandwidth 2 */
SFRX(AX5051_FSKDEV0,               0x4027)   /* FSK Deviation 0 */
SFRX(AX5051_FSKDEV1,               0x4026)   /* FSK Deviation 1 */
SFRX(AX5051_FSKDEV2,               0x4025)   /* FSK Deviation 2 */
SFRX(AX5051_IFFREQHI,              0x4028)   /* IF Frequency Low */
SFRX(AX5051_IFFREQLO,              0x4029)   /* IF Frequency High */
SFRX(AX5051_IFMODE,                0x4008)   /* Interface Mode */
SFRX(AX5051_IRQINVERSION,          0x400F)   /* IRQ Inversion */
SFRX(AX5051_IRQMASK,               0x4006)   /* IRQ Mask */
SFRX(AX5051_IRQREQUEST,            0x4007)   /* IRQ Request */
SFRX(AX5051_MODULATION,            0x4010)   /* Modulation */
SFRX(AX5051_MODULATORMISC,         0x4034)   /* Modulator Miscellaneous Control */
SFRX(AX5051_PHASEGAIN,             0x4044)   /* Phase Estimator Bandwidth */
SFRX(AX5051_PINCFG1,               0x400C)   /* Pin Configuration 1 */
SFRX(AX5051_PINCFG2,               0x400D)   /* Pin Configuration 2 */
SFRX(AX5051_PINCFG3,               0x400E)   /* Pin Configuration 3 */
SFRX(AX5051_PLLLOOP,               0x402C)   /* PLL Loop Filter */
SFRX(AX5051_PLLRANGING,            0x402D)   /* PLL Autoranging Control */
SFRX(AX5051_PLLRNGCLK,             0x402E)   /* PLL Autoranging Clock */
SFRX(AX5051_PLLVCOI,               0x4072)   /* PLL VCO Current */
SFRX(AX5051_PWRMODE,               0x4002)   /* Power Mode */
SFRX(AX5051_REF,                   0x407C)   /* Reference */
SFRX(AX5051_RFMISC,                0x407A)   /* RF Miscellaneous Control */
SFRX(AX5051_RXMISC,                0x407D)   /* Receiver Miscellaneous Control */
SFRX(AX5051_SCRATCH,               0x4001)   /* Scratch */
SFRX(AX5051_SILICONREVISION,       0x4000)   /* Silicon Revision */
SFRX(AX5051_TIMINGGAINHI,          0x4042)   /* Timing Estimator Bandwidth High */
SFRX(AX5051_TIMINGGAINLO,          0x4043)   /* Timing Estimator Bandwidth Low */
SFRX(AX5051_TRKAMPLITUDEHI,        0x4048)   /* Amplitude Tracking High */
SFRX(AX5051_TRKAMPLITUDELO,        0x4049)   /* Amplitude Tracking Low */
SFRX(AX5051_TRKFREQHI,             0x404C)   /* Frequency Tracking High */
SFRX(AX5051_TRKFREQLO,             0x404D)   /* Frequency Tracking Low */
SFRX(AX5051_TRKPHASEHI,            0x404A)   /* Phase Tracking High */
SFRX(AX5051_TRKPHASELO,            0x404B)   /* Phase Tracking Low */
SFRX(AX5051_TXBITRATEHI,           0x4031)   /* Transmitter Bitrate High */
SFRX(AX5051_TXBITRATELO,           0x4033)   /* Transmitter Bitrate Low */
SFRX(AX5051_TXBITRATEMID,          0x4032)   /* Transmitter Bitrate Middle */
SFRX(AX5051_TXDSPMODE,             0x400A)   /* Transmit DSP Mode */
SFRX(AX5051_TXPWR,                 0x4030)   /* Transmit Power */
SFRX(AX5051_VREG,                  0x401B)   /* Voltage Regulator */
SFRX(AX5051_XTALCAP,               0x404F)   /* Crystal Oscillator Load Capacitance */
SFRX(AX5051_XTALOSC,               0x4003)   /* Crystal Oscillator Control */

/* Radio Registers, X Address Space, Non-Blocking Version */

SFRX(AX5051_ADCMISCNB,             0x5038)   /* ADC Miscellaneous Control, Non-Blocking */
SFRX(AX5051_AGCATTACKNB,           0x503A)   /* AGC Attack Speed, Non-Blocking */
SFRX(AX5051_AGCCOUNTERNB,          0x503C)   /* AGC Counter, Non-Blocking */
SFRX(AX5051_AGCDECAYNB,            0x503B)   /* AGC Decay Speed, Non-Blocking */
SFRX(AX5051_AGCTARGETNB,           0x5039)   /* AGC Target Value, Non-Blocking */
SFRX(AX5051_AMPLITUDEGAINNB,       0x5047)   /* Amplitude Estimator Bandwidth, Non-Blocking */
SFRX(AX5051_CICDECHINB,            0x503E)   /* Decimation Factor High, Non-Blocking */
SFRX(AX5051_CICDECLONB,            0x503F)   /* Decimation Factor Low, Non-Blocking */
SFRX(AX5051_CICSHIFTNB,            0x503D)   /* Decimation Filter Attenuation, Non-Blocking */
SFRX(AX5051_CRCINIT0NB,            0x5017)   /* CRC Initial Value 0, Non-Blocking */
SFRX(AX5051_CRCINIT1NB,            0x5016)   /* CRC Initial Value 1, Non-Blocking */
SFRX(AX5051_CRCINIT2NB,            0x5015)   /* CRC Initial Value 2, Non-Blocking */
SFRX(AX5051_CRCINIT3NB,            0x5014)   /* CRC Initial Value 3, Non-Blocking */
SFRX(AX5051_DATARATEHINB,          0x5040)   /* Datarate High, Non-Blocking */
SFRX(AX5051_DATARATELONB,          0x5041)   /* Datarate Low, Non-Blocking */
SFRX(AX5051_DSPMODENB,             0x5009)   /* DSP Mode Interface Control, Non-Blocking */
SFRX(AX5051_ENCODINGNB,            0x5011)   /* Encoding, Non-Blocking */
SFRX(AX5051_FECNB,                 0x5018)   /* Forward Error Correction, Non-Blocking */
SFRX(AX5051_FECMEMNB,              0x506F)   /* Forward Error Correction Memory, Non-Blocking */
SFRX(AX5051_FECSTATUSNB,           0x501A)   /* Forward Error Correction Status, Non-Blocking */
SFRX(AX5051_FECSYNCNB,             0x5019)   /* Forward Error Correction Sync Threshold, Non-Blocking */
SFRX(AX5051_FIFOCONTROLNB,         0x5004)   /* FIFO Control, Non-Blocking */
SFRX(AX5051_FIFOCONTROL2NB,        0x5037)   /* FIFO Control 2, Non-Blocking */
SFRX(AX5051_FIFOCOUNTNB,           0x5035)   /* FIFO Count, Non-Blocking */
SFRX(AX5051_FIFODATANB,            0x5005)   /* FIFO Data, Non-Blocking */
SFRX(AX5051_FIFOTHRESHNB,          0x5036)   /* FIFO Threshold, Non-Blocking */
SFRX(AX5051_FRAMINGNB,             0x5012)   /* Framing Mode, Non-Blocking */
SFRX(AX5051_FREQ0NB,               0x5023)   /* Frequency 0, Non-Blocking */
SFRX(AX5051_FREQ1NB,               0x5022)   /* Frequency 1, Non-Blocking */
SFRX(AX5051_FREQ2NB,               0x5021)   /* Frequency 2, Non-Blocking */
SFRX(AX5051_FREQ3NB,               0x5020)   /* Frequency 3, Non-Blocking */
SFRX(AX5051_FREQA0NB,              0x5023)   /* Frequency 0, Non-Blocking */
SFRX(AX5051_FREQA1NB,              0x5022)   /* Frequency 1, Non-Blocking */
SFRX(AX5051_FREQA2NB,              0x5021)   /* Frequency 2, Non-Blocking */
SFRX(AX5051_FREQA3NB,              0x5020)   /* Frequency 3, Non-Blocking */
SFRX(AX5051_FREQUENCYGAINNB,       0x5045)   /* Frequency Estimator Bandwidth, Non-Blocking */
SFRX(AX5051_FREQUENCYGAIN2NB,      0x5046)   /* Frequency Estimator Bandwidth 2, Non-Blocking */
SFRX(AX5051_FSKDEV0NB,             0x5027)   /* FSK Deviation 0, Non-Blocking */
SFRX(AX5051_FSKDEV1NB,             0x5026)   /* FSK Deviation 1, Non-Blocking */
SFRX(AX5051_FSKDEV2NB,             0x5025)   /* FSK Deviation 2, Non-Blocking */
SFRX(AX5051_IFFREQHINB,            0x5028)   /* IF Frequency Low, Non-Blocking */
SFRX(AX5051_IFFREQLONB,            0x5029)   /* IF Frequency High, Non-Blocking */
SFRX(AX5051_IFMODENB,              0x5008)   /* Interface Mode, Non-Blocking */
SFRX(AX5051_IRQINVERSIONNB,        0x500F)   /* IRQ Inversion, Non-Blocking */
SFRX(AX5051_IRQMASKNB,             0x5006)   /* IRQ Mask, Non-Blocking */
SFRX(AX5051_IRQREQUESTNB,          0x5007)   /* IRQ Request, Non-Blocking */
SFRX(AX5051_MODULATIONNB,          0x5010)   /* Modulation, Non-Blocking */
SFRX(AX5051_MODULATORMISCNB,       0x5034)   /* Modulator Miscellaneous Control, Non-Blocking */
SFRX(AX5051_PHASEGAINNB,           0x5044)   /* Phase Estimator Bandwidth, Non-Blocking */
SFRX(AX5051_PINCFG1NB,             0x500C)   /* Pin Configuration 1, Non-Blocking */
SFRX(AX5051_PINCFG2NB,             0x500D)   /* Pin Configuration 2, Non-Blocking */
SFRX(AX5051_PINCFG3NB,             0x500E)   /* Pin Configuration 3, Non-Blocking */
SFRX(AX5051_PLLLOOPNB,             0x502C)   /* PLL Loop Filter, Non-Blocking */
SFRX(AX5051_PLLRANGINGNB,          0x502D)   /* PLL Autoranging Control, Non-Blocking */
SFRX(AX5051_PLLRNGCLKNB,           0x502E)   /* PLL Autoranging Clock, Non-Blocking */
SFRX(AX5051_PLLVCOINB,             0x5072)   /* PLL VCO Current, Non-Blocking */
SFRX(AX5051_PWRMODENB,             0x5002)   /* Power Mode, Non-Blocking */
SFRX(AX5051_REFNB,                 0x507C)   /* Reference, Non-Blocking */
SFRX(AX5051_RFMISCNB,              0x507A)   /* RF Miscellaneous Control, Non-Blocking */
SFRX(AX5051_RXMISCNB,              0x507D)   /* Receiver Miscellaneous Control, Non-Blocking */
SFRX(AX5051_SCRATCHNB,             0x5001)   /* Scratch, Non-Blocking */
SFRX(AX5051_SILICONREVISIONNB,     0x5000)   /* Silicon Revision, Non-Blocking */
SFRX(AX5051_TIMINGGAINHINB,        0x5042)   /* Timing Estimator Bandwidth High, Non-Blocking */
SFRX(AX5051_TIMINGGAINLONB,        0x5043)   /* Timing Estimator Bandwidth Low, Non-Blocking */
SFRX(AX5051_TRKAMPLITUDEHINB,      0x5048)   /* Amplitude Tracking High, Non-Blocking */
SFRX(AX5051_TRKAMPLITUDELONB,      0x5049)   /* Amplitude Tracking Low, Non-Blocking */
SFRX(AX5051_TRKFREQHINB,           0x504C)   /* Frequency Tracking High, Non-Blocking */
SFRX(AX5051_TRKFREQLONB,           0x504D)   /* Frequency Tracking Low, Non-Blocking */
SFRX(AX5051_TRKPHASEHINB,          0x504A)   /* Phase Tracking High, Non-Blocking */
SFRX(AX5051_TRKPHASELONB,          0x504B)   /* Phase Tracking Low, Non-Blocking */
SFRX(AX5051_TXBITRATEHINB,         0x5031)   /* Transmitter Bitrate High, Non-Blocking */
SFRX(AX5051_TXBITRATELONB,         0x5033)   /* Transmitter Bitrate Low, Non-Blocking */
SFRX(AX5051_TXBITRATEMIDNB,        0x5032)   /* Transmitter Bitrate Middle, Non-Blocking */
SFRX(AX5051_TXDSPMODENB,           0x500A)   /* Transmit DSP Mode, Non-Blocking */
SFRX(AX5051_TXPWRNB,               0x5030)   /* Transmit Power, Non-Blocking */
SFRX(AX5051_VREGNB,                0x501B)   /* Voltage Regulator, Non-Blocking */
SFRX(AX5051_XTALCAPNB,             0x504F)   /* Crystal Oscillator Load Capacitance, Non-Blocking */
SFRX(AX5051_XTALOSCNB,             0x5003)   /* Crystal Oscillator Control, Non-Blocking */

/* Alternate Names */

#define AX5051_AMPLGAIN            AX5051_AMPLITUDEGAIN
#define AX5051_FREQGAIN            AX5051_FREQUENCYGAIN
#define AX5051_FREQGAIN2           AX5051_FREQUENCYGAIN2
#define AX5051_MODMISC             AX5051_MODULATORMISC
#define AX5051_TMGGAINHI           AX5051_TIMINGGAINHI
#define AX5051_TMGGAINLO           AX5051_TIMINGGAINLO
#define AX5051_TXRATEHI            AX5051_TXBITRATEHI
#define AX5051_TXRATELO            AX5051_TXBITRATELO
#define AX5051_TXRATEMID           AX5051_TXBITRATEMID

#define AX5051_AMPLGAINNB          AX5051_AMPLITUDEGAINNB
#define AX5051_FREQGAINNB          AX5051_FREQUENCYGAINNB
#define AX5051_FREQGAIN2NB         AX5051_FREQUENCYGAIN2NB
#define AX5051_MODMISCNB           AX5051_MODULATORMISCNB
#define AX5051_TMGGAINHINB         AX5051_TIMINGGAINHINB
#define AX5051_TMGGAINLONB         AX5051_TIMINGGAINLONB
#define AX5051_TXRATEHINB          AX5051_TXBITRATEHINB
#define AX5051_TXRATELONB          AX5051_TXBITRATELONB
#define AX5051_TXRATEMIDNB         AX5051_TXBITRATEMIDNB

#endif /* AX8052F151_H */

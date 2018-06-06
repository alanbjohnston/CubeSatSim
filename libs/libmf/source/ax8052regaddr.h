/*-------------------------------------------------------------------------
   ax8052regaddr.h - Register Address Declarations for the Axsem Microfoot Processor Range

   Copyright (C) 2010, 2011, 2012 Axsem AG
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


#ifndef AX8052REGADDR_H
#define AX8052REGADDR_H

/* SFR Address Space */

#define SFRADDR_ACC              0xE0    /* Accumulator */
#define SFRADDR_B                0xF0    /* B Register */
#define SFRADDR_DPH              0x83    /* Data Pointer 0 High Byte */
#define SFRADDR_DPH1             0x85    /* Data Pointer 1 High Byte */
#define SFRADDR_DPL              0x82    /* Data Pointer 0 Low Byte */
#define SFRADDR_DPL1             0x84    /* Data Pointer 1 Low Byte */
#define SFRADDR_DPS              0x86    /* Data Pointer Select */
#define SFRADDR_E2IE             0xA0    /* 2nd Extended Interrupt Enable */
#define SFRADDR_E2IP             0xC0    /* 2nd Extended Interrupt Priority */
#define SFRADDR_EIE              0x98    /* Extended Interrupt Enable */
#define SFRADDR_EIP              0xB0    /* Extended Interrupt Priority */
#define SFRADDR_IE               0xA8    /* Interrupt Enable */
#define SFRADDR_IP               0xB8    /* Interrupt Priority */
#define SFRADDR_PCON             0x87    /* Power Mode Control */
#define SFRADDR_PSW              0xD0    /* Program Status Word */
#define SFRADDR_SP               0x81    /* Stack Pointer */
#define SFRADDR_WBTEST           0x8F    /* Debug Breakpoint Register */
#define SFRADDR_XPAGE            0xD9    /* Memory Page Select */
#define SFRADDR_ADCCH0CONFIG     0xCA    /* ADC Channel 0 Configuration */
#define SFRADDR_ADCCH1CONFIG     0xCB    /* ADC Channel 1 Configuration */
#define SFRADDR_ADCCH2CONFIG     0xD2    /* ADC Channel 2 Configuration */
#define SFRADDR_ADCCH3CONFIG     0xD3    /* ADC Channel 3 Configuration */
#define SFRADDR_ADCCLKSRC        0xD1    /* ADC Clock Source */
#define SFRADDR_ADCCONV          0xC9    /* ADC Conversion Source */
#define SFRADDR_ANALOGCOMP       0xE1    /* Analog Comparators */
#define SFRADDR_CLKCON           0xC6    /* Clock Control */
#define SFRADDR_CLKSTAT          0xC7    /* Clock Status */
#define SFRADDR_CODECONFIG       0x97    /* Code Space Configuration */
#define SFRADDR_DBGLNKBUF        0xE3    /* Debug Link Buffer */
#define SFRADDR_DBGLNKSTAT       0xE2    /* Debug Link Status */
#define SFRADDR_DIRA             0x89    /* Port A Direction */
#define SFRADDR_DIRB             0x8A    /* Port B Direction */
#define SFRADDR_DIRC             0x8B    /* Port C Direction */
#define SFRADDR_DIRR             0x8E    /* Port R Direction */
#define SFRADDR_PINA             0xC8    /* Port A Input */
#define SFRADDR_PINB             0xE8    /* Port B Input */
#define SFRADDR_PINC             0xF8    /* Port C Input */
#define SFRADDR_PINR             0x8D    /* Port R Input */
#define SFRADDR_PORTA            0x80    /* Port A Output */
#define SFRADDR_PORTB            0x88    /* Port B Output */
#define SFRADDR_PORTC            0x90    /* Port C Output */
#define SFRADDR_PORTR            0x8C    /* Port R Output */
#define SFRADDR_IC0CAPT0         0xCE    /* Input Capture 0 Low Byte */
#define SFRADDR_IC0CAPT1         0xCF    /* Input Capture 0 High Byte */
#define SFRADDR_IC0MODE          0xCC    /* Input Capture 0 Mode */
#define SFRADDR_IC0STATUS        0xCD    /* Input Capture 0 Status */
#define SFRADDR_IC1CAPT0         0xD6    /* Input Capture 1 Low Byte */
#define SFRADDR_IC1CAPT1         0xD7    /* Input Capture 1 High Byte */
#define SFRADDR_IC1MODE          0xD4    /* Input Capture 1 Mode */
#define SFRADDR_IC1STATUS        0xD5    /* Input Capture 1 Status */
#define SFRADDR_NVADDR0          0x92    /* Non-Volatile Memory Address Low Byte */
#define SFRADDR_NVADDR1          0x93    /* Non-Volatile Memory Address High Byte */
#define SFRADDR_NVDATA0          0x94    /* Non-Volatile Memory Data Low Byte */
#define SFRADDR_NVDATA1          0x95    /* Non-Volatile Memory Data High Byte */
#define SFRADDR_NVKEY            0x96    /* Non-Volatile Memory Write/Erase Key */
#define SFRADDR_NVSTATUS         0x91    /* Non-Volatile Memory Command / Status  */
#define SFRADDR_OC0COMP0         0xBC    /* Output Compare 0 Low Byte */
#define SFRADDR_OC0COMP1         0xBD    /* Output Compare 0 High Byte */
#define SFRADDR_OC0MODE          0xB9    /* Output Compare 0 Mode */
#define SFRADDR_OC0PIN           0xBA    /* Output Compare 0 Pin Configuration */
#define SFRADDR_OC0STATUS        0xBB    /* Output Compare 0 Status */
#define SFRADDR_OC1COMP0         0xC4    /* Output Compare 1 Low Byte */
#define SFRADDR_OC1COMP1         0xC5    /* Output Compare 1 High Byte */
#define SFRADDR_OC1MODE          0xC1    /* Output Compare 1 Mode */
#define SFRADDR_OC1PIN           0xC2    /* Output Compare 1 Pin Configuration */
#define SFRADDR_OC1STATUS        0xC3    /* Output Compare 1 Status */
#define SFRADDR_RADIOACC         0xB1    /* Radio Controller Access Mode */
#define SFRADDR_RADIOADDR0       0xB3    /* Radio Register Address Low Byte */
#define SFRADDR_RADIOADDR1       0xB2    /* Radio Register Address High Byte */
#define SFRADDR_RADIODATA0       0xB7    /* Radio Register Data 0 */
#define SFRADDR_RADIODATA1       0xB6    /* Radio Register Data 1 */
#define SFRADDR_RADIODATA2       0xB5    /* Radio Register Data 2 */
#define SFRADDR_RADIODATA3       0xB4    /* Radio Register Data 3 */
#define SFRADDR_RADIOSTAT0       0xBE    /* Radio Access Status Low Byte */
#define SFRADDR_RADIOSTAT1       0xBF    /* Radio Access Status High Byte */
#define SFRADDR_SPCLKSRC         0xDF    /* SPI Clock Source */
#define SFRADDR_SPMODE           0xDC    /* SPI Mode */
#define SFRADDR_SPSHREG          0xDE    /* SPI Shift Register */
#define SFRADDR_SPSTATUS         0xDD    /* SPI Status */
#define SFRADDR_T0CLKSRC         0x9A    /* Timer 0 Clock Source */
#define SFRADDR_T0CNT0           0x9C    /* Timer 0 Count Low Byte */
#define SFRADDR_T0CNT1           0x9D    /* Timer 0 Count High Byte */
#define SFRADDR_T0MODE           0x99    /* Timer 0 Mode */
#define SFRADDR_T0PERIOD0        0x9E    /* Timer 0 Period Low Byte */
#define SFRADDR_T0PERIOD1        0x9F    /* Timer 0 Period High Byte */
#define SFRADDR_T0STATUS         0x9B    /* Timer 0 Status */
#define SFRADDR_T1CLKSRC         0xA2    /* Timer 1 Clock Source */
#define SFRADDR_T1CNT0           0xA4    /* Timer 1 Count Low Byte */
#define SFRADDR_T1CNT1           0xA5    /* Timer 1 Count High Byte */
#define SFRADDR_T1MODE           0xA1    /* Timer 1 Mode */
#define SFRADDR_T1PERIOD0        0xA6    /* Timer 1 Period Low Byte */
#define SFRADDR_T1PERIOD1        0xA7    /* Timer 1 Period High Byte */
#define SFRADDR_T1STATUS         0xA3    /* Timer 1 Status */
#define SFRADDR_T2CLKSRC         0xAA    /* Timer 2 Clock Source */
#define SFRADDR_T2CNT0           0xAC    /* Timer 2 Count Low Byte */
#define SFRADDR_T2CNT1           0xAD    /* Timer 2 Count High Byte */
#define SFRADDR_T2MODE           0xA9    /* Timer 2 Mode */
#define SFRADDR_T2PERIOD0        0xAE    /* Timer 2 Period Low Byte */
#define SFRADDR_T2PERIOD1        0xAF    /* Timer 2 Period High Byte */
#define SFRADDR_T2STATUS         0xAB    /* Timer 2 Status */
#define SFRADDR_U0CTRL           0xE4    /* UART 0 Control */
#define SFRADDR_U0MODE           0xE7    /* UART 0 Mode */
#define SFRADDR_U0SHREG          0xE6    /* UART 0 Shift Register */
#define SFRADDR_U0STATUS         0xE5    /* UART 0 Status */
#define SFRADDR_U1CTRL           0xEC    /* UART 1 Control */
#define SFRADDR_U1MODE           0xEF    /* UART 1 Mode */
#define SFRADDR_U1SHREG          0xEE    /* UART 1 Shift Register */
#define SFRADDR_U1STATUS         0xED    /* UART 1 Status */
#define SFRADDR_WDTCFG           0xDA    /* Watchdog Configuration */
#define SFRADDR_WDTRESET         0xDB    /* Watchdog Reset */
#define SFRADDR_WTCFGA           0xF1    /* Wakeup Timer A Configuration */
#define SFRADDR_WTCFGB           0xF9    /* Wakeup Timer B Configuration */
#define SFRADDR_WTCNTA0          0xF2    /* Wakeup Counter A Low Byte */
#define SFRADDR_WTCNTA1          0xF3    /* Wakeup Counter A High Byte */
#define SFRADDR_WTCNTB0          0xFA    /* Wakeup Counter B Low Byte */
#define SFRADDR_WTCNTB1          0xFB    /* Wakeup Counter B High Byte */
#define SFRADDR_WTCNTR1          0xEB    /* Wakeup Counter High Byte Latch */
#define SFRADDR_WTEVTA0          0xF4    /* Wakeup Event A Low Byte */
#define SFRADDR_WTEVTA1          0xF5    /* Wakeup Event A High Byte */
#define SFRADDR_WTEVTB0          0xF6    /* Wakeup Event B Low Byte */
#define SFRADDR_WTEVTB1          0xF7    /* Wakeup Event B High Byte */
#define SFRADDR_WTEVTC0          0xFC    /* Wakeup Event C Low Byte */
#define SFRADDR_WTEVTC1          0xFD    /* Wakeup Event C High Byte */
#define SFRADDR_WTEVTD0          0xFE    /* Wakeup Event D Low Byte */
#define SFRADDR_WTEVTD1          0xFF    /* Wakeup Event D High Byte */
#define SFRADDR_WTIRQEN          0xE9    /* Wakeup Timer Interrupt Enable */
#define SFRADDR_WTSTAT           0xEA    /* Wakeup Timer Status */

/* X Address Space */


#define SFRXADDR_ADCCALG00GAIN0  0x7030  /* ADC Calibration Range 00 Gain Low Byte */
#define SFRXADDR_ADCCALG00GAIN1  0x7031  /* ADC Calibration Range 00 Gain High Byte */
#define SFRXADDR_ADCCALG01GAIN0  0x7032  /* ADC Calibration Range 01 Gain Low Byte */
#define SFRXADDR_ADCCALG01GAIN1  0x7033  /* ADC Calibration Range 01 Gain High Byte */
#define SFRXADDR_ADCCALG10GAIN0  0x7034  /* ADC Calibration Range 10 Gain Low Byte */
#define SFRXADDR_ADCCALG10GAIN1  0x7035  /* ADC Calibration Range 10 Gain High Byte */
#define SFRXADDR_ADCCALTEMPGAIN0 0x7038  /* ADC Calibration Temperature Gain Low Byte */
#define SFRXADDR_ADCCALTEMPGAIN1 0x7039  /* ADC Calibration Temperature Gain High Byte */
#define SFRXADDR_ADCCALTEMPOFFS0 0x703A  /* ADC Calibration Temperature Offset Low Byte */
#define SFRXADDR_ADCCALTEMPOFFS1 0x703B  /* ADC Calibration Temperature Offset High Byte */
#define SFRXADDR_ADCCH0VAL0      0x7020  /* ADC Channel 0 Low Byte */
#define SFRXADDR_ADCCH0VAL1      0x7021  /* ADC Channel 0 High Byte */
#define SFRXADDR_ADCCH1VAL0      0x7022  /* ADC Channel 1 Low Byte */
#define SFRXADDR_ADCCH1VAL1      0x7023  /* ADC Channel 1 High Byte */
#define SFRXADDR_ADCCH2VAL0      0x7024  /* ADC Channel 2 Low Byte */
#define SFRXADDR_ADCCH2VAL1      0x7025  /* ADC Channel 2 High Byte */
#define SFRXADDR_ADCCH3VAL0      0x7026  /* ADC Channel 3 Low Byte */
#define SFRXADDR_ADCCH3VAL1      0x7027  /* ADC Channel 3 High Byte */
#define SFRXADDR_ADCTUNE0        0x7028  /* ADC Tuning 0 */
#define SFRXADDR_ADCTUNE1        0x7029  /* ADC Tuning 1 */
#define SFRXADDR_ADCTUNE2        0x702A  /* ADC Tuning 2 */
#define SFRXADDR_CLOCKGATE       0x7F1B  /* Clock Gating */
#define SFRXADDR_DMA0ADDR0       0x7010  /* DMA Channel 0 Address Low Byte */
#define SFRXADDR_DMA0ADDR1       0x7011  /* DMA Channel 0 Address High Byte */
#define SFRXADDR_DMA0CONFIG      0x7014  /* DMA Channel 0 Configuration */
#define SFRXADDR_DMA1ADDR0       0x7012  /* DMA Channel 1 Address Low Byte */
#define SFRXADDR_DMA1ADDR1       0x7013  /* DMA Channel 1 Address High Byte */
#define SFRXADDR_DMA1CONFIG      0x7015  /* DMA Channel 1 Configuration */
#define SFRXADDR_FRCOSCCONFIG    0x7070  /* Fast RC Oscillator Calibration Configuration */
#define SFRXADDR_FRCOSCCTRL      0x7071  /* Fast RC Oscillator Control */
#define SFRXADDR_FRCOSCFREQ0     0x7076  /* Fast RC Oscillator Frequency Tuning Low Byte */
#define SFRXADDR_FRCOSCFREQ1     0x7077  /* Fast RC Oscillator Frequency Tuning High Byte */
#define SFRXADDR_FRCOSCKFILT0    0x7072  /* Fast RC Oscillator Calibration Filter Constant Low Byte */
#define SFRXADDR_FRCOSCKFILT1    0x7073  /* Fast RC Oscillator Calibration Filter Constant High Byte */
#define SFRXADDR_FRCOSCPER0      0x7078  /* Fast RC Oscillator Period Low Byte */
#define SFRXADDR_FRCOSCPER1      0x7079  /* Fast RC Oscillator Period High Byte */
#define SFRXADDR_FRCOSCREF0      0x7074  /* Fast RC Oscillator Reference Frequency Low Byte */
#define SFRXADDR_FRCOSCREF1      0x7075  /* Fast RC Oscillator Reference Frequency High Byte */
#define SFRXADDR_ANALOGA         0x7007  /* Port A Analog Mode */
#define SFRXADDR_GPIOENABLE      0x700C  /* GPIO Port Enable */
#define SFRXADDR_EXTIRQ          0x7003  /* External IRQ Configuration */
#define SFRXADDR_INTCHGA         0x7000  /* Port A Interrupt on Change */
#define SFRXADDR_INTCHGB         0x7001  /* Port B Interrupt on Change */
#define SFRXADDR_INTCHGC         0x7002  /* Port C Interrupt on Change */
#define SFRXADDR_PALTA           0x7008  /* Port A Alternate Function */
#define SFRXADDR_PALTB           0x7009  /* Port B Alternate Function */
#define SFRXADDR_PALTC           0x700A  /* Port C Alternate Function */
#define SFRXADDR_PALTRADIO       0x7046  /* Port Radio Alternate Function */
#define SFRXADDR_PINCHGA         0x7004  /* Port A Level Change */
#define SFRXADDR_PINCHGB         0x7005  /* Port B Level Change */
#define SFRXADDR_PINCHGC         0x7006  /* Port C Level Change */
#define SFRXADDR_PINSEL          0x700B  /* Port Input Selection */
#define SFRXADDR_LPOSCCONFIG     0x7060  /* Low Power Oscillator Calibration Configuration */
#define SFRXADDR_LPOSCFREQ0      0x7066  /* Low Power Oscillator Frequency Tuning Low Byte */
#define SFRXADDR_LPOSCFREQ1      0x7067  /* Low Power Oscillator Frequency Tuning High Byte */
#define SFRXADDR_LPOSCKFILT0     0x7062  /* Low Power Oscillator Calibration Filter Constant Low Byte */
#define SFRXADDR_LPOSCKFILT1     0x7063  /* Low Power Oscillator Calibration Filter Constant High Byte */
#define SFRXADDR_LPOSCPER0       0x7068  /* Low Power Oscillator Period Low Byte */
#define SFRXADDR_LPOSCPER1       0x7069  /* Low Power Oscillator Period High Byte */
#define SFRXADDR_LPOSCREF0       0x7064  /* Low Power Oscillator Reference Frequency Low Byte */
#define SFRXADDR_LPOSCREF1       0x7065  /* Low Power Oscillator Reference Frequency High Byte */
#define SFRXADDR_LPXOSCGM        0x7054  /* Low Power Crystal Oscillator Transconductance */
#define SFRXADDR_MISCCTRL        0x7F01  /* Miscellaneous Control */
#define SFRXADDR_OSCCALIB        0x7053  /* Oscillator Calibration Interrupt / Status */
#define SFRXADDR_OSCFORCERUN     0x7050  /* Oscillator Run Force */
#define SFRXADDR_OSCREADY        0x7052  /* Oscillator Ready Status */
#define SFRXADDR_OSCRUN          0x7051  /* Oscillator Run Status */
#define SFRXADDR_POWCTRL0        0x7F10  /* Power Control 0 */
#define SFRXADDR_POWCTRL1        0x7F11  /* Power Control 1 */
#define SFRXADDR_POWCTRL2        0x7F12  /* Power Control 2 */
#define SFRXADDR_RADIOFDATAADDR0 0x7040  /* Radio FIFO Data Register Address Low Byte */
#define SFRXADDR_RADIOFDATAADDR1 0x7041  /* Radio FIFO Data Register Address High Byte */
#define SFRXADDR_RADIOFSTATADDR0 0x7042  /* Radio FIFO Status Register Address Low Byte */
#define SFRXADDR_RADIOFSTATADDR1 0x7043  /* Radio FIFO Status Register Address High Byte */
#define SFRXADDR_RADIOMUX        0x7044  /* Radio Multiplexer Control */
#define SFRXADDR_SCRATCH0        0x7084  /* Scratch Register 0 */
#define SFRXADDR_SCRATCH1        0x7085  /* Scratch Register 1 */
#define SFRXADDR_SCRATCH2        0x7086  /* Scratch Register 2 */
#define SFRXADDR_SCRATCH3        0x7087  /* Scratch Register 3 */
#define SFRXADDR_SILICONREV      0x7F00  /* Silicon Revision */
#define SFRXADDR_XTALAMPL        0x7F19  /* Crystal Oscillator Amplitude Control */
#define SFRXADDR_XTALOSC         0x7F18  /* Crystal Oscillator Configuration */
#define SFRXADDR_XTALREADY       0x7F1A  /* Crystal Oscillator Ready Mode */

/* X Address Space aliases of SFR Address Space Registers */

#define SFRXADDR_XIE             0x3FA8  /* Interrupt Enable */
#define SFRXADDR_XIP             0x3FB8  /* Interrupt Priority */
#define SFRXADDR_XPCON           0x3F87  /* Power Mode Control */
#define SFRXADDR_XADCCH0CONFIG   0x3FCA  /* ADC Channel 0 Configuration */
#define SFRXADDR_XADCCH1CONFIG   0x3FCB  /* ADC Channel 1 Configuration */
#define SFRXADDR_XADCCH2CONFIG   0x3FD2  /* ADC Channel 2 Configuration */
#define SFRXADDR_XADCCH3CONFIG   0x3FD3  /* ADC Channel 3 Configuration */
#define SFRXADDR_XADCCLKSRC      0x3FD1  /* ADC Clock Source */
#define SFRXADDR_XADCCONV        0x3FC9  /* ADC Conversion Source */
#define SFRXADDR_XANALOGCOMP     0x3FE1  /* Analog Comparators */
#define SFRXADDR_XCLKCON         0x3FC6  /* Clock Control */
#define SFRXADDR_XCLKSTAT        0x3FC7  /* Clock Status */
#define SFRXADDR_XCODECONFIG     0x3F97  /* Code Space Configuration */
#define SFRXADDR_XDBGLNKBUF      0x3FE3  /* Debug Link Buffer */
#define SFRXADDR_XDBGLNKSTAT     0x3FE2  /* Debug Link Status */
#define SFRXADDR_XDIRA           0x3F89  /* Port A Direction */
#define SFRXADDR_XDIRB           0x3F8A  /* Port B Direction */
#define SFRXADDR_XDIRC           0x3F8B  /* Port C Direction */
#define SFRXADDR_XDIRR           0x3F8E  /* Port R Direction */
#define SFRXADDR_XPINA           0x3FC8  /* Port A Input */
#define SFRXADDR_XPINB           0x3FE8  /* Port B Input */
#define SFRXADDR_XPINC           0x3FF8  /* Port C Input */
#define SFRXADDR_XPINR           0x3F8D  /* Port R Input */
#define SFRXADDR_XPORTA          0x3F80  /* Port A Output */
#define SFRXADDR_XPORTB          0x3F88  /* Port B Output */
#define SFRXADDR_XPORTC          0x3F90  /* Port C Output */
#define SFRXADDR_XPORTR          0x3F8C  /* Port R Output */
#define SFRXADDR_XIC0CAPT0       0x3FCE  /* Input Capture 0 Low Byte */
#define SFRXADDR_XIC0CAPT1       0x3FCF  /* Input Capture 0 High Byte */
#define SFRXADDR_XIC0MODE        0x3FCC  /* Input Capture 0 Mode */
#define SFRXADDR_XIC0STATUS      0x3FCD  /* Input Capture 0 Status */
#define SFRXADDR_XIC1CAPT0       0x3FD6  /* Input Capture 1 Low Byte */
#define SFRXADDR_XIC1CAPT1       0x3FD7  /* Input Capture 1 High Byte */
#define SFRXADDR_XIC1MODE        0x3FD4  /* Input Capture 1 Mode */
#define SFRXADDR_XIC1STATUS      0x3FD5  /* Input Capture 1 Status */
#define SFRXADDR_XNVADDR0        0x3F92  /* Non-Volatile Memory Address Low Byte */
#define SFRXADDR_XNVADDR1        0x3F93  /* Non-Volatile Memory Address High Byte */
#define SFRXADDR_XNVDATA0        0x3F94  /* Non-Volatile Memory Data Low Byte */
#define SFRXADDR_XNVDATA1        0x3F95  /* Non-Volatile Memory Data High Byte */
#define SFRXADDR_XNVKEY          0x3F96  /* Non-Volatile Memory Write/Erase Key */
#define SFRXADDR_XNVSTATUS       0x3F91  /* Non-Volatile Memory Command / Status  */
#define SFRXADDR_XOC0COMP0       0x3FBC  /* Output Compare 0 Low Byte */
#define SFRXADDR_XOC0COMP1       0x3FBD  /* Output Compare 0 High Byte */
#define SFRXADDR_XOC0MODE        0x3FB9  /* Output Compare 0 Mode */
#define SFRXADDR_XOC0PIN         0x3FBA  /* Output Compare 0 Pin Configuration */
#define SFRXADDR_XOC0STATUS      0x3FBB  /* Output Compare 0 Status */
#define SFRXADDR_XOC1COMP0       0x3FC4  /* Output Compare 1 Low Byte */
#define SFRXADDR_XOC1COMP1       0x3FC5  /* Output Compare 1 High Byte */
#define SFRXADDR_XOC1MODE        0x3FC1  /* Output Compare 1 Mode */
#define SFRXADDR_XOC1PIN         0x3FC2  /* Output Compare 1 Pin Configuration */
#define SFRXADDR_XOC1STATUS      0x3FC3  /* Output Compare 1 Status */
#define SFRXADDR_XRADIOACC       0x3FB1  /* Radio Controller Access Mode */
#define SFRXADDR_XRADIOADDR0     0x3FB3  /* Radio Register Address Low Byte */
#define SFRXADDR_XRADIOADDR1     0x3FB2  /* Radio Register Address High Byte */
#define SFRXADDR_XRADIODATA0     0x3FB7  /* Radio Register Data 0 */
#define SFRXADDR_XRADIODATA1     0x3FB6  /* Radio Register Data 1 */
#define SFRXADDR_XRADIODATA2     0x3FB5  /* Radio Register Data 2 */
#define SFRXADDR_XRADIODATA3     0x3FB4  /* Radio Register Data 3 */
#define SFRXADDR_XRADIOSTAT0     0x3FBE  /* Radio Access Status Low Byte */
#define SFRXADDR_XRADIOSTAT1     0x3FBF  /* Radio Access Status High Byte */
#define SFRXADDR_XSPCLKSRC       0x3FDF  /* SPI Clock Source */
#define SFRXADDR_XSPMODE         0x3FDC  /* SPI Mode */
#define SFRXADDR_XSPSHREG        0x3FDE  /* SPI Shift Register */
#define SFRXADDR_XSPSTATUS       0x3FDD  /* SPI Status */
#define SFRXADDR_XT0CLKSRC       0x3F9A  /* Timer 0 Clock Source */
#define SFRXADDR_XT0CNT0         0x3F9C  /* Timer 0 Count Low Byte */
#define SFRXADDR_XT0CNT1         0x3F9D  /* Timer 0 Count High Byte */
#define SFRXADDR_XT0MODE         0x3F99  /* Timer 0 Mode */
#define SFRXADDR_XT0PERIOD0      0x3F9E  /* Timer 0 Period Low Byte */
#define SFRXADDR_XT0PERIOD1      0x3F9F  /* Timer 0 Period High Byte */
#define SFRXADDR_XT0STATUS       0x3F9B  /* Timer 0 Status */
#define SFRXADDR_XT1CLKSRC       0x3FA2  /* Timer 1 Clock Source */
#define SFRXADDR_XT1CNT0         0x3FA4  /* Timer 1 Count Low Byte */
#define SFRXADDR_XT1CNT1         0x3FA5  /* Timer 1 Count High Byte */
#define SFRXADDR_XT1MODE         0x3FA1  /* Timer 1 Mode */
#define SFRXADDR_XT1PERIOD0      0x3FA6  /* Timer 1 Period Low Byte */
#define SFRXADDR_XT1PERIOD1      0x3FA7  /* Timer 1 Period High Byte */
#define SFRXADDR_XT1STATUS       0x3FA3  /* Timer 1 Status */
#define SFRXADDR_XT2CLKSRC       0x3FAA  /* Timer 2 Clock Source */
#define SFRXADDR_XT2CNT0         0x3FAC  /* Timer 2 Count Low Byte */
#define SFRXADDR_XT2CNT1         0x3FAD  /* Timer 2 Count High Byte */
#define SFRXADDR_XT2MODE         0x3FA9  /* Timer 2 Mode */
#define SFRXADDR_XT2PERIOD0      0x3FAE  /* Timer 2 Period Low Byte */
#define SFRXADDR_XT2PERIOD1      0x3FAF  /* Timer 2 Period High Byte */
#define SFRXADDR_XT2STATUS       0x3FAB  /* Timer 2 Status */
#define SFRXADDR_XU0CTRL         0x3FE4  /* UART 0 Control */
#define SFRXADDR_XU0MODE         0x3FE7  /* UART 0 Mode */
#define SFRXADDR_XU0SHREG        0x3FE6  /* UART 0 Shift Register */
#define SFRXADDR_XU0STATUS       0x3FE5  /* UART 0 Status */
#define SFRXADDR_XU1CTRL         0x3FEC  /* UART 1 Control */
#define SFRXADDR_XU1MODE         0x3FEF  /* UART 1 Mode */
#define SFRXADDR_XU1SHREG        0x3FEE  /* UART 1 Shift Register */
#define SFRXADDR_XU1STATUS       0x3FED  /* UART 1 Status */
#define SFRXADDR_XWDTCFG         0x3FDA  /* Watchdog Configuration */
#define SFRXADDR_XWDTRESET       0x3FDB  /* Watchdog Reset */
#define SFRXADDR_XWTCFGA         0x3FF1  /* Wakeup Timer A Configuration */
#define SFRXADDR_XWTCFGB         0x3FF9  /* Wakeup Timer B Configuration */
#define SFRXADDR_XWTCNTA0        0x3FF2  /* Wakeup Counter A Low Byte */
#define SFRXADDR_XWTCNTA1        0x3FF3  /* Wakeup Counter A High Byte */
#define SFRXADDR_XWTCNTB0        0x3FFA  /* Wakeup Counter B Low Byte */
#define SFRXADDR_XWTCNTB1        0x3FFB  /* Wakeup Counter B High Byte */
#define SFRXADDR_XWTCNTR1        0x3FEB  /* Wakeup Counter High Byte Latch */
#define SFRXADDR_XWTEVTA0        0x3FF4  /* Wakeup Event A Low Byte */
#define SFRXADDR_XWTEVTA1        0x3FF5  /* Wakeup Event A High Byte */
#define SFRXADDR_XWTEVTB0        0x3FF6  /* Wakeup Event B Low Byte */
#define SFRXADDR_XWTEVTB1        0x3FF7  /* Wakeup Event B High Byte */
#define SFRXADDR_XWTEVTC0        0x3FFC  /* Wakeup Event C Low Byte */
#define SFRXADDR_XWTEVTC1        0x3FFD  /* Wakeup Event C High Byte */
#define SFRXADDR_XWTEVTD0        0x3FFE  /* Wakeup Event D Low Byte */
#define SFRXADDR_XWTEVTD1        0x3FFF  /* Wakeup Event D High Byte */
#define SFRXADDR_XWTIRQEN        0x3FE9  /* Wakeup Timer Interrupt Enable */
#define SFRXADDR_XWTSTAT         0x3FEA  /* Wakeup Timer Status */

#endif /* AX8052REGADDR_H */

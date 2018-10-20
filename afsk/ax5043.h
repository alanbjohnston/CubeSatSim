/*
 *  A sample application transmitting AFSK at 1200 baud
 *
 *  Portions Copyright (C) 2018 Libre Space Foundation
 *  Portions Copyright (C) 2018 Jonathan Brandenburg
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AX5043_H_
#define AX5043_H_

#include <stdint.h>

#define FREQUENCY_OFFSET                -80000

#define APRS_VHF                        440390000

/******************************************************************************
 ************************* RF Configuration ***********************************
 *****************************************************************************/
#define RX_FREQ_HZ                      (APRS_VHF + FREQUENCY_OFFSET)
#define TX_FREQ_HZ                      (APRS_VHF + FREQUENCY_OFFSET)

/* Reference Oscillator frequency */
#define XTAL_FREQ_HZ                    16000000

/**
 * The maximum allowed frame size
 */
#define MAX_FRAME_LEN                   1024

#define RX_BAUDRATE                     1200
#define TX_BAUDRATE                     1200

#define MIN_RF_FREQ_INT_VCO_RFDIV0      800000000
#define MAX_RF_FREQ_INT_VCO_RFDIV0      1050000000

#define MIN_RF_FREQ_INT_VCO_RFDIV1      (MIN_RF_FREQ_INT_VCO_RFDIV0 / 2)
#define MAX_RF_FREQ_INT_VCO_RFDIV1      (MAX_RF_FREQ_INT_VCO_RFDIV0 / 2)

#define MIN_RF_FREQ_EXT_VCO_RFDIV0      54000000
#define MAX_RF_FREQ_EXT_VCO_RFDIV0      525000000

#define MIN_RF_FREQ_EXT_VCO_RFDIV1      (MIN_RF_FREQ_EXT_VCO_RFDIV0 / 2)
#define MAX_RF_FREQ_EXT_VCO_RFDIV1      (MAX_RF_FREQ_EXT_VCO_RFDIV0 / 2)

/**
 * Ramp up/Ramp down period of the power amplifier in microseconds
 */
#define PWRAMP_RAMP_PERIOD_US           200

/******************************************************************************
 ********************  AX5043 control SPI registers ***************************
 *****************************************************************************/

/* Status and test registers */
#define AX5043_REG_REV                  0x0
#define AX5043_REG_SCRATCH              0x1

/* Power and voltage regulator */
#define AX5043_REG_PWRMODE              0x2
#define AX5043_REG_POWSTAT              0x3
#define AX5043_REG_POWSTICKYSTAT        0x4
#define AX5043_REG_POWIRQMASK           0x5

/* Interrupt control */
#define AX5043_REG_IRQMASK1             0x6
#define AX5043_REG_IRQMASK0             0x7
#define AX5043_REG_RADIOEVENTMASK1      0x8
#define AX5043_REG_RADIOEVENTMASK0      0x9

#define AX5043_REG_IRQREQUEST1          0xC
#define AX5043_REG_IRQREQUEST0          0xD
#define AX5043_REG_RADIOEVENTREQ1       0xE
#define AX5043_REG_RADIOEVENTREQ0       0xF

/* Modulation and framing */
#define AX5043_REG_MODULATION           0x010
#define AX5043_REG_ENCODING             0x011
#define AX5043_REG_FRAMING              0x012
#define AX5043_REG_CRCINIT3             0x014
#define AX5043_REG_CRCINIT2             0x015
#define AX5043_REG_CRCINIT1             0x016
#define AX5043_REG_CRCINIT0             0x017

/* FEC */
#define AX5043_REG_FEC                  0x018
#define AX5043_REG_FECSYNC              0x019
#define AX5043_REG_FECSTATUS            0x01A

/* Status */
#define AX5043_REG_RADIOSTATE           0x01C
#define AX5043_REG_XTALSTATUS           0x01D

/* Pin configuration */
#define AX5043_REG_PINSTATE             0x20
#define AX5043_REG_PINFUNCSYSCLK        0x21
#define AX5043_REG_PINFUNCDCLK          0x22
#define AX5043_REG_PINFUNCDATA          0x23
#define AX5043_REG_PINFUNCIRQ           0x24
#define AX5043_REG_PINFUNCANTSEL        0x25
#define AX5043_REG_PINFUNCPWRAMP        0x26
#define AX5043_REG_PWRAMP               0x27

/* FIFO control */
#define AX5043_REG_FIFOSTAT             0x28
#define AX5043_REG_FIFODATA             0x29
#define AX5043_REG_FIFOCOUNT1           0x2A
#define AX5043_REG_FIFOCOUNT0           0x2B
#define AX5043_REG_FIFOFREE1            0x2C
#define AX5043_REG_FIFOFREE0            0x2D
#define AX5043_REG_FIFOTHRESH1          0x2E
#define AX5043_REG_FIFOTHRESH0          0x2F

/* Frequency Synthesizer */
#define AX5043_REG_PLLLOOP              0x30
#define AX5043_REG_PLLCPI               0x31
#define AX5043_REG_PLLVCODIV            0x32
#define AX5043_REG_PLLRANGINGA          0x33
#define AX5043_REG_FREQA3               0x34
#define AX5043_REG_FREQA2               0x35
#define AX5043_REG_FREQA1               0x36
#define AX5043_REG_FREQA0               0x37
#define AX5043_REG_PLLLOOPBOOST         0x38
#define AX5043_REG_PLLCPIBOOST          0x39
#define AX5043_REG_PLLRANGINGB          0x3B
#define AX5043_REG_FREQB3               0x3C
#define AX5043_REG_FREQB2               0x3D
#define AX5043_REG_FREQB1               0x3E
#define AX5043_REG_FREQB0               0x3F

/* RSSI */
#define AX5043_REG_RSSI                 0x40
#define AX5043_REG_BGNDRSSI             0x41
#define AX5043_REG_DIVERSITY            0x42
#define AX5043_REG_AGCCOUNTER           0x43

/* Receiver Tracking */
#define AX5043_REG_TRKDATARATE2         0x45
#define AX5043_REG_TRKDATARATE1         0x46
#define AX5043_REG_TRKDATARATE0         0x47
#define AX5043_REG_TRKAMPL1             0x48
#define AX5043_REG_TRKAMPL0             0x49
#define AX5043_REG_TRKPHASE1            0x4A
#define AX5043_REG_TRKPHASE0            0x4B
#define AX5043_REG_TRKRFFREQ2           0x4D
#define AX5043_REG_TRKRFFREQ1           0x4E
#define AX5043_REG_TRKRFFREQ0           0x4F
#define AX5043_REG_TRKFREQ1             0x50
#define AX5043_REG_TRKFREQ0             0x51
#define AX5043_REG_TRKFSKDEMOD1         0x52
#define AX5043_REG_TRKFSKDEMOD0         0x53

/* Timers */
#define AX5043_REG_TIMER2               0x59
#define AX5043_REG_TIMER1               0x5A
#define AX5043_REG_TIMER0               0x5B

/* Wakeup timer */
#define AX5043_REG_WAKEUPTIMER1         0x68
#define AX5043_REG_WAKEUPTIMER0         0x69
#define AX5043_REG_WAKEUP1              0x6A
#define AX5043_REG_WAKEUP0              0x6B
#define AX5043_REG_WAKEUPFREQ1          0x6C
#define AX5043_REG_WAKEUPFREQ0          0x6D
#define AX5043_REG_WAKEUPXOEARLY        0x6E

/* PHY related registers*/
#define AX5043_REG_IFFREQ1              0x100
#define AX5043_REG_IFFREQ0              0x101
#define AX5043_REG_DECIMATION           0x102
#define AX5043_REG_RXDATARATE2          0x103
#define AX5043_REG_RXDATARATE1          0x104
#define AX5043_REG_RXDATARATE0          0x105
#define AX5043_REG_MAXDROFFSET2         0x106
#define AX5043_REG_MAXDROFFSET1         0x107
#define AX5043_REG_MAXDROFFSET0         0x108
#define AX5043_REG_MAXRFOFFSET2         0x109
#define AX5043_REG_MAXRFOFFSET1         0x10A
#define AX5043_REG_MAXRFOFFSET0         0x10B
#define AX5043_REG_FSKDMAX1             0x10C
#define AX5043_REG_FSKDMAX0             0x10D
#define AX5043_REG_FSKDMIN1             0x10E
#define AX5043_REG_FSKDMIN0             0x10F
#define AX5043_REG_AFSKSPACE1           0x110
#define AX5043_REG_AFSKSPACE0           0x111
#define AX5043_REG_AFSKMARK1            0x112
#define AX5043_REG_AFSKMARK0            0x113
#define AX5043_REG_AFSKCTRL             0x114
#define AX5043_REG_AMPLFILTER           0x115
#define AX5043_REG_FREQUENCYLEAK        0x116
#define AX5043_REG_RXPARAMSETS          0x117
#define AX5043_REG_RXPARAMCURSET        0x118

/* Receiver Parameter Set 0 */
#define AX5043_REG_AGCGAIN0             0x120
#define AX5043_REG_AGCTARGET0           0x121
#define AX5043_REG_AGCAHYST0            0x122
#define AX5043_REG_AGCMINMAX0           0x123
#define AX5043_REG_TIMEGAIN0            0x124
#define AX5043_REG_DRGAIN0              0x125
#define AX5043_REG_PHASEGAIN0           0x126
#define AX5043_REG_FREQGAINA0           0x127
#define AX5043_REG_FREQGAINB0           0x128
#define AX5043_REG_FREQGAINC0           0x129
#define AX5043_REG_FREQGAIND0           0x12A
#define AX5043_REG_AMPLGAIN0            0x12B
#define AX5043_REG_FREQDEV10            0x12C
#define AX5043_REG_FREQDEV00            0x12D
#define AX5043_REG_FOURFSK0             0x12E
#define AX5043_REG_BBOFFSRES0           0x12F

/* Receiver Parameter Set 1 */
#define AX5043_REG_AGCGAIN1             0x130
#define AX5043_REG_AGCTARGET1           0x131
#define AX5043_REG_AGCAHYST1            0x132
#define AX5043_REG_AGCMINMAX1           0x133
#define AX5043_REG_TIMEGAIN1            0x134
#define AX5043_REG_DRGAIN1              0x135
#define AX5043_REG_PHASEGAIN1           0x136
#define AX5043_REG_FREQGAINA1           0x137
#define AX5043_REG_FREQGAINB1           0x138
#define AX5043_REG_FREQGAINC1           0x139
#define AX5043_REG_FREQGAIND1           0x13A
#define AX5043_REG_AMPLGAIN1            0x13B
#define AX5043_REG_FREQDEV11            0x13C
#define AX5043_REG_FREQDEV01            0x13D
#define AX5043_REG_FOURFSK1             0x13E
#define AX5043_REG_BBOFFSRES1           0x13F

/* Receiver Parameter Set 2 */
#define AX5043_REG_AGCGAIN2             0x140
#define AX5043_REG_AGCTARGET2           0x141
#define AX5043_REG_AGCAHYST2            0x142
#define AX5043_REG_AGCMINMAX2           0x143
#define AX5043_REG_TIMEGAIN2            0x144
#define AX5043_REG_DRGAIN2              0x145
#define AX5043_REG_PHASEGAIN2           0x146
#define AX5043_REG_FREQGAINA2           0x147
#define AX5043_REG_FREQGAINB2           0x148
#define AX5043_REG_FREQGAINC2           0x149
#define AX5043_REG_FREQGAIND2           0x14A
#define AX5043_REG_AMPLGAIN2            0x14B
#define AX5043_REG_FREQDEV12            0x14C
#define AX5043_REG_FREQDEV02            0x14D
#define AX5043_REG_FOURFSK2             0x14E
#define AX5043_REG_BBOFFSRES2           0x14F

/* Receiver Parameter Set 3 */
#define AX5043_REG_AGCGAIN3             0x150
#define AX5043_REG_AGCTARGET3           0x151
#define AX5043_REG_AGCAHYST3            0x152
#define AX5043_REG_AGCMINMAX3           0x153
#define AX5043_REG_TIMEGAIN3            0x154
#define AX5043_REG_DRGAIN3              0x155
#define AX5043_REG_PHASEGAIN3           0x156
#define AX5043_REG_FREQGAINA3           0x157
#define AX5043_REG_FREQGAINB3           0x158
#define AX5043_REG_FREQGAINC3           0x159
#define AX5043_REG_FREQGAIND3           0x15A
#define AX5043_REG_AMPLGAIN3            0x15B
#define AX5043_REG_FREQDEV13            0x15C
#define AX5043_REG_FREQDEV03            0x15D
#define AX5043_REG_FOURFSK3             0x15E
#define AX5043_REG_BBOFFSRES3           0x15F

/* Transmitter Parameters */
#define AX5043_REG_MODCFGF              0x160
#define AX5043_REG_FSKDEV2              0x161
#define AX5043_REG_FSKDEV1              0x162
#define AX5043_REG_FSKDEV0              0x163
#define AX5043_REG_MODCFGA              0x164
#define AX5043_REG_TXRATE2              0x165
#define AX5043_REG_TXRATE1              0x166
#define AX5043_REG_TXRATE0              0x167
#define AX5043_REG_TXPWRCOEFFA1         0x168
#define AX5043_REG_TXPWRCOEFFA0         0x169
#define AX5043_REG_TXPWRCOEFFB1         0x16A
#define AX5043_REG_TXPWRCOEFFB0         0x16B
#define AX5043_REG_TXPWRCOEFFC1         0x16C
#define AX5043_REG_TXPWRCOEFFC0         0x16D
#define AX5043_REG_TXPWRCOEFFD1         0x16E
#define AX5043_REG_TXPWRCOEFFD0         0x16F
#define AX5043_REG_TXPWRCOEFFE1         0x170
#define AX5043_REG_TXPWRCOEFFE0         0x171

/* PLL parameters */
#define AX5043_REG_PLLVCOI              0x180
#define AX5043_REG_PLLVCOIR             0x181
#define AX5043_REG_PLLLOCKDET           0x182
#define AX5043_REG_PLLRNGCLK            0x183

/* Crystal Oscillator */
#define AX5043_REG_XTALCAP              0x184

/* Baseband */
#define AX5043_REG_BBTUNE               0x188
#define AX5043_REG_BBOFFSCAP            0x189

/* MAC parameters */

/* Packet Format */
#define AX5043_REG_PKTADDRCFG           0x200
#define AX5043_REG_PKTLENCFG            0x201
#define AX5043_REG_PKTLENOFFSET         0x202
#define AX5043_REG_PKTMAXLEN            0x203
#define AX5043_REG_PKTADDR3             0x204
#define AX5043_REG_PKTADDR2             0x205
#define AX5043_REG_PKTADDR1             0x206
#define AX5043_REG_PKTADDR0             0x207
#define AX5043_REG_PKTADDRMASK3         0x208
#define AX5043_REG_PKTADDRMASK2         0x209
#define AX5043_REG_PKTADDRMASK1         0x20A
#define AX5043_REG_PKTADDRMASK0         0x20B

/* Pattern Match */
#define AX5043_REG_MATCH0PAT3           0x210
#define AX5043_REG_MATCH0PAT2           0x211
#define AX5043_REG_MATCH0PAT1           0x212
#define AX5043_REG_MATCH0PAT0           0x213
#define AX5043_REG_MATCH0LEN            0x214
#define AX5043_REG_MATCH0MIN            0x215
#define AX5043_REG_MATCH0MAX            0x216
#define AX5043_REG_MATCH1PAT1           0x218
#define AX5043_REG_MATCH1PAT0           0x219
#define AX5043_REG_MATCH1LEN            0x21C
#define AX5043_REG_MATCH1MIN            0x21D
#define AX5043_REG_MATCH1MAX            0x21E

/* Packet Controller */
#define AX5043_REG_TMGTXBOOST           0x220
#define AX5043_REG_TMGTXSETTLE          0x221
#define AX5043_REG_TMGRXBOOST           0x223
#define AX5043_REG_TMGRXSETTLE          0x224
#define AX5043_REG_TMGRXOFFSACQ         0x225
#define AX5043_REG_TMGRXCOARSEAGC       0x226
#define AX5043_REG_TMGRXAGC             0x227
#define AX5043_REG_TMGRXRSSI            0x228
#define AX5043_REG_TMGRXPREAMBLE1       0x229
#define AX5043_REG_TMGRXPREAMBLE2       0x22A
#define AX5043_REG_TMGRXPREAMBLE3       0x22B
#define AX5043_REG_RSSIREFERENCE        0x22C
#define AX5043_REG_RSSIABSTHR           0x22D
#define AX5043_REG_BGNDRSSIGAIN         0x22E
#define AX5043_REG_BGNDRSSITHR          0x22F
#define AX5043_REG_PKTCHUNKSIZE         0x230
#define AX5043_REG_PKTMISCFLAGS         0x231
#define AX5043_REG_PKTSTOREFLAGS        0x232
#define AX5043_REG_PKTACCEPTFLAGS       0x233

/* Special Functions */

/* General Purpose ADC */
#define AX5043_REG_GPADCCTRL            0x300
#define AX5043_REG_GPADCPERIOD          0x301
#define AX5043_REG_GPADC13VALUE1        0x308
#define AX5043_REG_GPADC13VALUE0        0x309

/* Low Power Oscillator Calibration */
#define AX5043_REG_LPOSCCONFIG          0x310
#define AX5043_REG_LPOSCSTATUS          0x311
#define AX5043_REG_LPOSCKFILT1          0x312
#define AX5043_REG_LPOSCKFILT0          0x313
#define AX5043_REG_LPOSCREF1            0x314
#define AX5043_REG_LPOSCREF0            0x315
#define AX5043_REG_LPOSCFREQ1           0x316
#define AX5043_REG_LPOSCFREQ0           0x317
#define AX5043_REG_LPOSCPER1            0x318
#define AX5043_REG_LPOSCPER0            0x319

/* Performance Tuning Registers */
#define AX5043_REG_XTALDIV              0xF35

/******************************************************************************
 ************************* Register values ************************************
 *****************************************************************************/
#define AX5043_REV                      0x51
#define AX5043_SCRATCH_TEST             0xAA

/* Power modes */
#define AX5043_POWERDOWN                0x0
#define AX5043_DEEPSLEEP                BIT(0)
#define AX5043_STANDBY                  (BIT(2) | BIT(0))
#define AX5043_FIFO_ENABLED             (BIT(2) | BIT(1) |BIT(0))
#define AX5043_RECEIVE_MODE             (BIT(3))
#define AX5043_RECEIVER_RUNNING         (BIT(3) | BIT(0))
#define AX5043_RECEIVER_WOR             (BIT(3) | BIT(1) | BIT(0))
#define AX5043_TRANSMIT_MODE            (BIT(3) | BIT(2))
#define AX5043_TRANSMIT_RUNNING         (BIT(3) | BIT(2) | BIT(0))
#define AX5043_FULLTX                   AX5043_TRANSMIT_RUNNING

#define AX5043_PLLVCOI_MANUAL           BIT(7)

/**
 * Modem Domain Voltage Regulator Ready
 */
#define AX5043_SVMODEM                  BIT(3)

/**
 * Init value for the VCO prior starting an autoranging
 */
#define AX5043_VCOR_INIT                8

#define AX5043_RFDIV0                   0x0
#define AX5043_RFDIV1                   BIT(2)

#define AX5043_FREQSHAPE_EXT_FILTER     0x0
#define AX5043_FREQSHAPE_INVALID        0x1
#define AX5043_FREQSHAPE_GAUSSIAN_BT_03 0x2
#define AX5043_FREQSHAPE_GAUSSIAN_BT_05 0x3

/**
 *  FSK modulation mode
 */
#define AX5043_MODULATION_FSK           BIT(3)

/**
 *  AFSK modulation mode
 */
#define AX5043_MODULATION_AFSK           (BIT(3)|BIT(1))

#define AX5043_ENC_INV                  BIT(0)
#define AX5043_ENC_DIFF                 BIT(1)
#define AX5043_ENC_SCRAM                BIT(2)

/**
 * HDLC Framing mode
 */
#define AX5043_HDLC_FRAMING             BIT(2)

/**
 * HDLC compliant CRC16
 */
#define AX5043_CRC16_CCITT              BIT(4)

/**
 * Set the FIFO to variable length data mode
 */
#define AX5043_FIFO_VARIABLE_DATA_CMD   0xe1

#define AX5043_FIFO_REPEATDATA_CMD      (BIT(6) | BIT(5) | BIT(1))

/**
 * FIFO commit command
 */
#define AX5043_FIFO_COMMIT_CMD          BIT(2)
#define AX5043_FIFO_PKTSTART            BIT(0)
#define AX5043_FIFO_PKTEND              BIT(1)
#define AX5043_FIFO_NOCRC               BIT(3)
#define AX5043_FIFO_RAW                 BIT(4)

/**
 * Maximum chuck that can be committed on the FIFO
 */
#define AX5043_PKTCHUNKSIZE_240         0xd

#define AX5043_FIFO_MAX_SIZE            240

/**
 * When this threshold of free bytes in the TX FIFO is reached,
 * we'll put more data in the FIFO
 */
#define AX5043_FIFO_FREE_THR            128

/**
 * TX antenna transmission mode
 */
#define AX5043_TX_SINGLE_ENDED          BIT(1)
#define AX5043_TX_DIFFERENTIAL          BIT(0)

/**
 * External PA Control
 */

#define AX5043_EXT_PA_ENABLE            1
#define AX5043_EXT_PA_DISABLE           0

/**
 * Frequency mode A or B actually selects at which registers
 * the frequency configuration should be written.
 *
 * This is quite handy for different RX/TX frequencies, to avoid
 * writing every time the two different frequency configurations.
 */
typedef enum {
    FREQA_MODE = 0,               //!< FREQA_MODE
    FREQB_MODE = 1                //!< FREQB_MODE
} freq_mode_t;

typedef enum {
    VCO_INTERNAL = 0, VCO_EXTERNAL = 1
} vco_mode_t;

typedef enum {
    POWERDOWN,
    DEEPSLEEP,
    STANDBY,
    FIFO_ENABLED,
    RECEIVE_MODE,
    RECEIVER_RUNNING,
    RECEIVER_WOR,
    TRANSMIT_MODE,
    FULLTX
} power_mode_t;

typedef struct {
    uint32_t tx_freq;
    uint32_t rx_freq;
    uint32_t f_xtal;
    uint8_t f_xtaldiv;
    uint32_t tx_baudrate;
    uint32_t rx_baudrate;
    uint8_t rf_init;
    freq_mode_t freqsel;
    vco_mode_t vco;
} ax5043_conf_t;

int ax5043_reset_a(ax5043_conf_t *conf);

int ax5043_init(ax5043_conf_t *conf, uint32_t f_xtal, vco_mode_t vco);

int ax5043_conf_tx_path(ax5043_conf_t *conf);

int ax5043_set_tx_freq(ax5043_conf_t *conf, uint32_t freq);

int ax5043_set_power_mode(ax5043_conf_t *conf, power_mode_t mode);

int ax5043_set_tx_baud(ax5043_conf_t *conf, uint32_t baud);

int ax5043_freqsel(ax5043_conf_t *conf, freq_mode_t f);

int ax5043_set_tx_synth(ax5043_conf_t *conf);

int ax5043_set_pll_params(ax5043_conf_t *conf);

int ax5043_autoranging(ax5043_conf_t *conf);

int ax5043_aprs_framing_setup(ax5043_conf_t *conf);

int ax5043_tx_frame(ax5043_conf_t *conf, const uint8_t *in, uint32_t len,
        uint8_t preamble_len, uint8_t postamble_len, uint32_t timeout_ms);

int ax5043_spi_wait_xtal(ax5043_conf_t *conf, uint32_t timeout_ms);

int ax5043_spi_read_8(ax5043_conf_t *conf, uint8_t *out, uint16_t reg);

int ax5043_spi_read_16(ax5043_conf_t *conf, uint16_t *out, uint16_t reg);

int ax5043_spi_read_24(ax5043_conf_t *conf, uint32_t *out, uint16_t reg);

int ax5043_spi_read_32(ax5043_conf_t *conf, uint32_t *out, uint16_t reg);

int ax5043_spi_write(ax5043_conf_t *conf, uint16_t reg, const uint8_t *in,
        uint32_t len);

int ax5043_spi_write_8(ax5043_conf_t *conf, uint16_t reg, uint8_t in);

int ax5043_spi_write_16(ax5043_conf_t *conf, uint16_t reg, uint16_t in);

int ax5043_spi_write_24(ax5043_conf_t *conf, uint16_t reg, uint32_t in);

int ax5043_spi_write_32(ax5043_conf_t *conf, uint16_t reg, uint32_t in);

int ax5043_enable_pwramp(ax5043_conf_t *conf, uint8_t enable);

int ax5043_set_antsel(ax5043_conf_t *conf, uint8_t val);

int ax5043_wait_for_transmit();

#endif /* AX5043_H_ */

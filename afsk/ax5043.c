/*
 *  A sample application transmitting AFSK 1200 baud
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

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "ax25.h"
#include "ax5043.h"
#include "status.h"
#include "utils.h"
#include "spi/ax5043spi.h"

static uint8_t single_fifo_access = 0;

static uint8_t __tx_buf[MAX_FRAME_LEN];
static size_t __tx_buf_idx = 0;
static uint8_t __tx_fifo_chunk[AX5043_FIFO_MAX_SIZE];
static uint32_t __tx_remaining = 0;

extern uint32_t tx_freq_hz;

/**
 * FIFO command for the preamble. The third byte corresponds the length of
 * the preamble and is set by the TX routine for every frame
 */
static uint8_t __preamble_cmd[4] = { AX5043_FIFO_REPEATDATA_CMD,
AX5043_FIFO_PKTSTART | AX5043_FIFO_RAW | AX5043_FIFO_NOCRC, 0,
AX25_SYNC_FLAG };

/**
 * FIFO command for the postable. The third byte corresponds the length of
 * the postable and is set by the TX routine for every frame
 */
static uint8_t __postamble_cmd[4] = {
AX5043_FIFO_REPEATDATA_CMD,
AX5043_FIFO_PKTSTART | AX5043_FIFO_PKTEND | AX5043_FIFO_RAW | AX5043_FIFO_NOCRC,
        0, AX25_SYNC_FLAG };

/**
 * Indicates if a TX is currently active
 */
static volatile uint8_t __tx_active = 0;

static ax5043_conf_t *__ax5043_conf = NULL;

static inline int set_tx_black_magic_regs();

/**
 * Checks if the AX5043 handler is valid
 * @param conf the AX5043 configuration handler pointer
 * @return 1 if it is valid 0 otherwise
 */
static uint8_t is_ax5043_conf_valid(ax5043_conf_t *conf) {
    if (!conf || !conf->f_xtal) {
        return 0;
    }
    return 1;
}

/**
 * Resets the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_reset_a(ax5043_conf_t *conf) {
    int ret;
    uint8_t val;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    conf->rf_init = 0;
    usleep(100);

    /* Reset the chip using the appropriate register */
    val = BIT(7);
    ret = ax5043_spi_write_8(conf, AX5043_REG_PWRMODE, val);
    if (ret) {
        return ret;
    }
    usleep(100);
    /* Clear the reset bit, but keep REFEN and XOEN */
    ret = ax5043_spi_read_8(conf, &val, AX5043_REG_PWRMODE);
    if (ret) {
        return ret;
    }
    val &= (BIT(6) | BIT(5));
    ret = ax5043_spi_write_8(conf, AX5043_REG_PWRMODE, val);
    if (ret) {
        return ret;
    }
    usleep(100);

    ret = ax5043_set_power_mode(conf, POWERDOWN);
    if (ret) {
        return ret;
    }
    return PQWS_SUCCESS;
}

/**
 * Initialization routine for the AX5043 IC
 * @param conf the AX5043 configuration handler
 * @param spi the SPI handler
 * @param f_xtal the frequency of the crystal or the TCXO
 * @param vco the VCO mode
 * @return 0 on success or appropriate negative error code
 */
int ax5043_init(ax5043_conf_t *conf, uint32_t f_xtal, vco_mode_t vco) {
    int ret;
    uint8_t revision;
    uint8_t val;

    if (!conf) {
        return -PQWS_INVALID_PARAM;
    }

    /* Set the initial parameters */
    memset(conf, 0, sizeof(ax5043_conf_t));

    switch (vco) {
    case VCO_INTERNAL:
    case VCO_EXTERNAL:
        conf->vco = vco;
        break;
    default:
        return -PQWS_INVALID_PARAM;
    }

    conf->rf_init = 0;
    conf->freqsel = -1;
    conf->f_xtal = f_xtal;
    if (conf->f_xtal > 24800000) {
        conf->f_xtaldiv = 2;
    } else {
        conf->f_xtaldiv = 1;
    }

    ret = ax5043_reset_a(conf);
    if (ret) {
        return ret;
    }

    /* Try first to read the revision register of the AX5043 */
    ret = ax5043_spi_read_8(conf, &revision, AX5043_REG_REV);
    if (ret) {
        return ret;
    }

    if (revision != AX5043_REV) {
        return -PQWS_NO_RF_FOUND;
    }

    /* To ensure communication try to write and read the scratch register */
    val = AX5043_SCRATCH_TEST;
    ret = ax5043_spi_write_8(conf, AX5043_REG_SCRATCH, val);
    if (ret) {
        return ret;
    }

    val = 0x0;
    ret = ax5043_spi_read_8(conf, &val, AX5043_REG_SCRATCH);
    if (ret) {
        return ret;
    }

    if (val != AX5043_SCRATCH_TEST) {
        return -PQWS_NO_RF_FOUND;
    }

    ret = ax5043_set_pll_params(conf);
    if (ret) {
        return ret;
    }

    /* Write the performance register F35 based on the XTAL frequency */
    if (conf->f_xtaldiv == 1) {
        ret = ax5043_spi_write_8(conf, 0xF35, 0x10);
    } else {
        ret = ax5043_spi_write_8(conf, 0xF35, 0x11);
    }
    if (ret) {
        return ret;
    }

    /* FIFO maximum chunk */
    ret = ax5043_spi_write_8(conf, AX5043_REG_PKTCHUNKSIZE,
    AX5043_PKTCHUNKSIZE_240);
    if (ret) {
        return ret;
    }

    /* Set RF parameters */
    ret = ax5043_freqsel(conf, FREQA_MODE);
    if (ret) {
        return ret;
    }

    /*
     * We use APRS for all transmitted frames. APRS is encapsulated in a
     * AX.25 frame. For 9600 baudrate is FSK9600 G3RUH compatible modem
     */
    ret = ax5043_aprs_framing_setup(conf);
    if (ret) {
        return ret;
    }

    /* Setup TX only related parameters */
    ret = ax5043_conf_tx_path(conf);
    if (ret) {
        return ret;
    }

    /* Set an internal copy for the ax5042_wait_for_transmit function */
    __ax5043_conf = conf;

    if (ret) {
        return ret;
    }
    ax5043_enable_pwramp(conf, AX5043_EXT_PA_DISABLE);
    return PQWS_SUCCESS;
}

/**
 * Performs TX specific configuration of the AX5043
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_conf_tx_path(ax5043_conf_t *conf) {
    int ret;

    ret = ax5043_set_tx_synth(conf);
    if (ret) {
        return ret;
    }

    ret = ax5043_set_tx_baud(conf, TX_BAUDRATE);
    if (ret) {
        return ret;
    }

    ret = ax5043_set_tx_freq(conf, tx_freq_hz);
    if (ret) {
        return ret;
    }

    /* Our TX is on single ended mode */
    //ret = ax5043_spi_write_8(conf, AX5043_REG_MODCFGA,
    //AX5043_TX_SINGLE_ENDED);
    //if (ret) {
    //    return ret;
    //}

    /* Our TX is on double ended mode */
    ret = ax5043_spi_write_8(conf, AX5043_REG_MODCFGA,
    AX5043_TX_DIFFERENTIAL);
    if (ret) {
        return ret;
    }

    /* Set the rest of the performance registers for TX */
    ret = set_tx_black_magic_regs(conf);
    if (ret) {
        return ret;
    }

    /*
     * As our board has an external PA, reduce the output power to reduce
     * the excess bandwidth emissions
     */
    //ret = ax5043_spi_write_16(conf, AX5043_REG_TXPWRCOEFFB1, 0x01FF);
    //if (ret) {
        //return ret;
    //}

    // Not using a PA, transmit half power
    ret = ax5043_spi_write_16(conf, AX5043_REG_TXPWRCOEFFB1, 0x07FF);
    if (ret) {
        return ret;
    }

    return PQWS_SUCCESS;
}

/**
 * Sets the power mode of the AX5043
 * @param conf the AX5043 configuration handler
 * @param mode the power mode
 * @return 0 on success or appropriate negative error code
 */
int ax5043_set_power_mode(ax5043_conf_t *conf, power_mode_t mode) {
    int ret;
    uint8_t val;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    /* Read the contents of the register */
    ret = ax5043_spi_read_8(conf, &val, AX5043_REG_PWRMODE);
    if (ret) {
        return ret;
    }

    /* Keep REFEN and XOEN values */
    val &= (BIT(6) | BIT(5));

    switch (mode) {
    case POWERDOWN:
        val |= AX5043_POWERDOWN;
        break;
    case DEEPSLEEP:
        val |= AX5043_DEEPSLEEP;
        break;
    case STANDBY:
        val |= AX5043_STANDBY;
        break;
    case FIFO_ENABLED:
        val |= AX5043_FIFO_ENABLED;
        break;
    case RECEIVE_MODE:
        val |= AX5043_RECEIVE_MODE;
        break;
    case RECEIVER_RUNNING:
        val |= AX5043_RECEIVER_RUNNING;
        break;
    case RECEIVER_WOR:
        val |= AX5043_RECEIVER_WOR;
        break;
    case TRANSMIT_MODE:
        val |= AX5043_TRANSMIT_MODE;
        break;
    case FULLTX:
        val |= AX5043_FULLTX;
        break;
    default:
        return -PQWS_INVALID_PARAM;
    }
    return ax5043_spi_write_8(conf, AX5043_REG_PWRMODE, val);
}

/**
 * Sets the RF frequency of the TX. If the previous TX frequency is
 * further enough than the new one, this function performs automatically
 * auto-ranging.
 *
 * @param conf the AX5043 configuration handler
 * @param freq the target RF frequency
 * @return 0 on success or appropriate negative error code
 */
int ax5043_set_tx_freq(ax5043_conf_t *conf, uint32_t freq) {
    int ret;
    uint32_t prev_freq;
    uint32_t reg_val;
    uint8_t rfdiv = 0;
    uint8_t pllcodediv = 0;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    /* Check the frequency range. The actual range depends on the VCO used */
    switch (conf->vco) {
    case VCO_INTERNAL:
        if (freq >= MIN_RF_FREQ_INT_VCO_RFDIV0
                && freq <= MAX_RF_FREQ_INT_VCO_RFDIV0) {
            rfdiv = AX5043_RFDIV0;
        } else if (freq >= MIN_RF_FREQ_INT_VCO_RFDIV1
                && freq <= MAX_RF_FREQ_INT_VCO_RFDIV1) {
            rfdiv = AX5043_RFDIV1;
        } else {
            return -PQWS_INVALID_PARAM;
        }
        break;
    case VCO_EXTERNAL:
        fprintf(stderr, "ERROR: Unexpected use of external VCO\n");
        if (freq >= MIN_RF_FREQ_EXT_VCO_RFDIV0
                && freq <= MAX_RF_FREQ_EXT_VCO_RFDIV0) {
            rfdiv = AX5043_RFDIV0;
        } else if (freq >= MIN_RF_FREQ_EXT_VCO_RFDIV1
                && freq <= MAX_RF_FREQ_EXT_VCO_RFDIV1) {
            rfdiv = AX5043_RFDIV1;
        } else {
            return -PQWS_INVALID_PARAM;
        }
        break;
    default:
        return -PQWS_INVALID_PARAM;
    }
    prev_freq = conf->tx_freq;
    pllcodediv = rfdiv | (uint8_t) (conf->vco << 4);

    // Added by Jonathan Brandenburg
    // Have an external inductor
    pllcodediv |= BIT(5);

    ret = ax5043_spi_write_8(conf, AX5043_REG_PLLVCODIV, pllcodediv);
    if (ret) {
        return ret;
    }

    /* Write properly the F34 performance register based on the RFDIV*/
    if (rfdiv == AX5043_RFDIV1) {
        ret = ax5043_spi_write_8(conf, 0xF34, 0x28);
    } else {
        ret = ax5043_spi_write_8(conf, 0xF34, 0x08);
    }
    if (ret) {
        return ret;
    }

    /*
     * Set the RF frequency
     * Frequency should be avoided to be a multiple integer of the crystal
     * frequency, so we always set to 1 the LSB
     */
    reg_val = ((uint32_t) (((float) freq / (float) conf->f_xtal) * (1 << 24))
            | 0x1);
    if (conf->freqsel == FREQA_MODE) {
        ret = ax5043_spi_write_32(conf, AX5043_REG_FREQA3, reg_val);
    } else {
        ret = ax5043_spi_write_32(conf, AX5043_REG_FREQB3, reg_val);
    }
    if (ret) {
        return ret;
    }

    /* Considered that the frequency successfully changed */
    conf->tx_freq = freq;

    /* If the frequency difference is great enough perform autoranging */
    if (freq + 25000000 > prev_freq || freq - 25000000 < prev_freq) {
        ax5043_autoranging(conf);
    }
        
    return PQWS_SUCCESS;
}

/**
 * Set the TX baudrate
 * @param conf the AX5043 configuration handler
 * @param baud the baudrate
 * @return 0 on success or negative error code
 */
int ax5043_set_tx_baud(ax5043_conf_t *conf, uint32_t baud) {
    int ret = PQWS_SUCCESS;
    uint32_t val;
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    val = (uint32_t) ((((float) baud) / (float) conf->f_xtal) * (1 << 24))
            | 0x1;
    ret = ax5043_spi_write_24(conf, AX5043_REG_TXRATE2, val);
    if (ret) {
        return ret;
    }

    conf->tx_baudrate = baud;

    /* Set the deviation to standard 3 KHz for FM */
    // For AFSK, FSKDEV = 0.858785 * fDeviation / fXTAL * 2^24
    //            
    val = (uint32_t) ((0.858785 * 3000.0f / (float) conf->f_xtal) * (1 << 24))
            | 0x1;
    ret = ax5043_spi_write_24(conf, AX5043_REG_FSKDEV2, val);
    if (ret) {
        return ret;
    }

    return PQWS_SUCCESS;
}

/**
 * Sets the currently used frequency registers (A or B)
 * @param conf the AX5043 configuration handler
 * @param f the frequency mode (A or B)
 * @return 0 on success or appropriate negative error code
 */
int ax5043_freqsel(ax5043_conf_t *conf, freq_mode_t f) {
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    if (f != FREQA_MODE && f != FREQB_MODE) {
        return -PQWS_INVALID_PARAM;
    }
    conf->freqsel = f;
    return PQWS_SUCCESS;
}

/**
 * Sets the TX frequency synthesizer related configuration registers.
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_set_tx_synth(ax5043_conf_t *conf) {
    int ret;
    uint8_t val;
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    switch (conf->freqsel) {
    case FREQA_MODE:
        val = 0x0;
        break;
    case FREQB_MODE:
        val = 1 << 7;
        break;
    default:
        return -PQWS_INVALID_PARAM;
    }

    /* Bypass external filter and use 100 kHZ loop bandwidth */
    val |= BIT(3) | BIT(0);
    ret = ax5043_spi_write_8(conf, AX5043_REG_PLLLOOP, val);
    if (ret) {
        return ret;
    }

    /*
     * Set the charge pump current based on the loop bandwidth
     * 68 uA @ 100 kHZ
     */
    ret = ax5043_spi_write_8(conf, AX5043_REG_PLLCPI, (uint8_t) (68 / 8.5));
    if (ret) {
        return ret;
    }
    ret = ax5043_spi_write_8(conf, AX5043_REG_XTALCAP, 0);
    return ret;
}

/**
 * Sets the PLL related configuration registers.
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_set_pll_params(ax5043_conf_t *conf) {
    int ret;
    uint8_t i = 8;
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    /* Set VCO to manual */
    ret = ax5043_spi_write_8(conf, AX5043_REG_PLLVCOI,
    AX5043_PLLVCOI_MANUAL | (1250 / 50));
    if (ret) {
        return ret;
    }

    /*
     * According to the manual PLL ranging clock should be less than 1/10
     * of the PLL loop bandwidth. The smallest PLL bandwidth configuration
     * is 100 kHz.
     */
    // This this next line contains an error
    //while (conf->f_xtal / (uint32_t) (1 << i) > 10000) {
    while (conf->f_xtal / (uint32_t) (1 << i) > 100000) {
        i++;
    }
    i = i > 15 ? 15 : i;
    i = i < 8 ? 8 : i;
    ret = ax5043_spi_write_8(conf, AX5043_REG_PLLRNGCLK, (uint8_t) (i - 8));
    return ret;
}

/**
 * Performs auto-ranging using the frequency registers configured by
 * ax5043_freqsel().
 *
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_autoranging(ax5043_conf_t *conf) {
    int ret = PQWS_SUCCESS;
    uint16_t pllranging_reg;
    uint8_t val = 0;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    switch (conf->freqsel) {
    case FREQA_MODE:
        pllranging_reg = AX5043_REG_PLLRANGINGA;
        break;
    case FREQB_MODE:
        pllranging_reg = AX5043_REG_PLLRANGINGB;
        break;
    default:
        return -PQWS_INVALID_PARAM;
    }

    /* Write the initial VCO setting and start autoranging */
    val = BIT(4) | AX5043_VCOR_INIT;
    ret = ax5043_spi_write_8(conf, pllranging_reg, val);
    if (ret) {
        printf("ERROR: AX5043 Autoranging Write Failure\n\n");
        return ret;
    }

    usleep(10);
    //val = 0;
    /* Wait until the autoranging is complete */
    int timeout = 0;
    clock_t start = clock();
    while (((val & BIT(4)) != 0) && !timeout ) {  // changed to !=, since https://www.onsemi.com/pub/Collateral/AND9347-D.PDF says BIT(4) RNG START clears when autoranging done
        ret = ax5043_spi_read_8(conf, &val, pllranging_reg);
        if (ret) {
            printf("ERROR: AX5043 Autoranging Read Failure\n\n");
            return ret;
        }
        if ((clock() - start) > 1000000) {
             timeout = 1;
        }
    }

    if (val & BIT(5)) {
        printf("ERROR: AX5043 Autoranging Error\n\n");
        return -PQWS_AX5043_AUTORANGING_ERROR;
    } else if (timeout) {
        printf("ERROR: AX5043 Autoranging Timeout\n\n");
        return -1;           
    }
    return PQWS_SUCCESS;
}

/**
 *
 * @param conf the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
int ax5043_aprs_framing_setup(ax5043_conf_t *conf) {
    int ret = PQWS_SUCCESS;
    uint8_t val = 0;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    /* Set modulation */
    val = AX5043_MODULATION_AFSK;
    ret = ax5043_spi_write_8(conf, AX5043_REG_MODULATION, val);
    if (ret) {
        return ret;
    }

    // To set the space frequency, 1070
    // 1070 * 2^18 / fXTAL -> 1070 * 2^18 / 16000000 -> 18 -> 0x12

    // To set the space frequency, 1200
    // 1200 * 2^18 / fXTAL -> 1200 * 2^18 / 16000000 -> 20 -> 0x14

    ret = ax5043_spi_write_16(conf, AX5043_REG_AFSKSPACE1, 0x24);
    if (ret) {
        return ret;
    }

    // To set the mark frequency, 1270
    // 1270 * 2^18 / fXTAL -> 1270 * 2^18 / 16000000 -> 21 -> 0x15

    // To set the mark frequency, 1400
    // 1400 * 2^18 / fXTAL -> 1400 * 2^18 / 16000000 -> 23 -> 0x17

    ret = ax5043_spi_write_16(conf, AX5043_REG_AFSKMARK1, 0x14);
    if (ret) {
        return ret;
    }

    /*
     * As we do not use any external filter, try to filter from
     * the AX5043 the signal
     */
    ret = ax5043_spi_write_8(conf, AX5043_REG_MODCFGF,
    AX5043_FREQSHAPE_GAUSSIAN_BT_05);
    if (ret) {
        return ret;
    }

    /* Set HDLC encoding: Differential = 1, Inverse = 1, Scrambling = 1 */
    //ax5043_spi_write_8(conf, AX5043_REG_ENCODING,
    /* Set HDLC encoding: Differential = 1, Inverse = 1, Scrambling = 1 */
    ax5043_spi_write_8(conf, AX5043_REG_ENCODING,
    AX5043_ENC_DIFF | AX5043_ENC_INV);

    /* HDLC framing */
    ax5043_spi_write_8(conf, AX5043_REG_FRAMING,
    AX5043_HDLC_FRAMING | AX5043_CRC16_CCITT);
    return ret;
}

static int __tx_frame_end(ax5043_conf_t *conf) {
    int ret;

    ax5043_enable_pwramp(conf, AX5043_EXT_PA_DISABLE);

    /* Set AX5043 to power down mode */
    ret = ax5043_set_power_mode(conf, POWERDOWN);
    __tx_active = 0;
    return ret;
}

static int __tx_frame(ax5043_conf_t *conf, const uint8_t *in, uint32_t len,
        uint8_t preamble_len, uint8_t postamble_len, uint32_t timeout_ms) {
    int ret = PQWS_SUCCESS;
    uint8_t data_cmd[3] = { AX5043_FIFO_VARIABLE_DATA_CMD, 0, 0 };
    size_t chunk_size = 0;
    size_t avail;
    uint8_t val;
    uint32_t start = millis();

    /*
     * Apply preamble and postamble repetition length. Rest of the fields should
     * remain unaltered
     */
    __preamble_cmd[2] = preamble_len;
    __postamble_cmd[2] = postamble_len;

    memcpy(__tx_fifo_chunk, __preamble_cmd, sizeof(__preamble_cmd));
    chunk_size = sizeof(__preamble_cmd);
    __tx_buf_idx = 0;

    /*
     * Always leave some space for the postamble. This greatly reduces the
     * complexity of dealing with some corner cases
     */
    avail = min_ul(
            AX5043_FIFO_MAX_SIZE - sizeof(__preamble_cmd) - sizeof(data_cmd)
                    - sizeof(__postamble_cmd), len);
    if (len == avail) {
        data_cmd[1] = (uint8_t) (len + 1);
        data_cmd[2] = AX5043_FIFO_PKTEND;
        __tx_remaining = 0;
        memcpy(__tx_fifo_chunk + chunk_size, data_cmd, sizeof(data_cmd));
        chunk_size += sizeof(data_cmd);
        memcpy(__tx_fifo_chunk + chunk_size, in, len);
        chunk_size += len;
        /*
         * At this point we are sure that the whole frame + postamble can fit in
         * the FIFO chunk
         */
        memcpy(__tx_fifo_chunk + chunk_size, __postamble_cmd,
                sizeof(__postamble_cmd));
        chunk_size += sizeof(__postamble_cmd);
        single_fifo_access = 1;
    } else {
        data_cmd[1] = (uint8_t) (avail + 1);
        data_cmd[2] = 0;
        memcpy(__tx_fifo_chunk + chunk_size, data_cmd, sizeof(data_cmd));
        chunk_size += sizeof(data_cmd);
        memcpy(__tx_fifo_chunk + chunk_size, in, avail);
        chunk_size += avail;

        memcpy(__tx_buf, in + avail, len - avail);
        __tx_remaining = (uint32_t) (len - avail);
        single_fifo_access = 0;
    }

    /* Set AX5043 to FULLTX mode */
    ret = ax5043_set_power_mode(conf, FULLTX);
    if (ret) {
        return ret;
    }

    ax5043_spi_wait_xtal(conf, 100);

    /* Wait for the FIFO to become ready */
    val = 0;
    while (!val) {
        ax5043_spi_read_8(conf, &val, AX5043_REG_POWSTAT);
        /* Select only the modem power state */
        val &= AX5043_SVMODEM;
        if (millis() - start > timeout_ms) {
            ret = -PQWS_TIMEOUT;
            break;
        }
    }

    /* Fire-up the first data to the FIFO */
    ret = ax5043_spi_write(conf, AX5043_REG_FIFODATA, __tx_fifo_chunk,
            (uint32_t) chunk_size);
    if (ret) {
        return ret;
    }
    __tx_active = 1;
    /* Commit to FIFO ! */
    ret = ax5043_spi_write_8(conf, AX5043_REG_FIFOSTAT, AX5043_FIFO_COMMIT_CMD);

    return ret;
}

int ax5043_tx_frame(ax5043_conf_t *conf, const uint8_t *in, uint32_t len,
        uint8_t preamble_len, uint8_t postamble_len, uint32_t timeout_ms) {
    int ret = 0;

    /* Wait for the previous frame to be transmitted */
    while (__tx_active) {
        ret++;
    }

    ret = ax5043_enable_pwramp(conf, AX5043_EXT_PA_ENABLE);
    if (ret) {
        return ret;
    }

    ret = __tx_frame(conf, in, len, preamble_len, postamble_len, timeout_ms);
    return ret;
}

/**
 * Wait the crystal to become ready
 * @param conf the AX5043 configuration handler
 * @param timeout_ms the timeout in milliseconds
 * @return 0 on success or appropriate negative error code
 */
int ax5043_spi_wait_xtal(ax5043_conf_t *conf, uint32_t timeout_ms) {
    int ret;
    uint8_t val = 0x0;
    uint32_t start = millis();

    while (!val) {
        ret = ax5043_spi_read_8(conf, &val, AX5043_REG_XTALSTATUS);
        if (ret) {
            return ret;
        }
        if ((millis() - start) > timeout_ms) {
            return -PQWS_TIMEOUT;
        }
    }
    return 0;
}

int ax5043_spi_read_8(ax5043_conf_t *conf, uint8_t *out, uint16_t reg) {
    int ret = PQWS_SUCCESS;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }
    *out = ax5043ReadReg(reg);

    return ret;
}

int ax5043_spi_read_16(ax5043_conf_t *conf, uint16_t *out, uint16_t reg) {
    int ret = PQWS_SUCCESS;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    *out = ax5043ReadReg2(reg);

    return ret;
}

int ax5043_spi_read_24(ax5043_conf_t *conf, uint32_t *out, uint16_t reg) {
    int ret = PQWS_SUCCESS;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    *out = ax5043ReadReg3(reg);

    return ret;
}

int ax5043_spi_read_32(ax5043_conf_t *conf, uint32_t *out, uint16_t reg) {
    int ret = PQWS_SUCCESS;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    *out = ax5043ReadReg4(reg);

    return ret;
}

int ax5043_spi_write(ax5043_conf_t *conf, uint16_t reg, const uint8_t *in,
        uint32_t len) {
    int ret = PQWS_SUCCESS;

    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    ax5043WriteRegN(reg, in, len);

    return ret;
}

int ax5043_spi_write_8(ax5043_conf_t *conf, uint16_t reg, uint8_t in) {
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    //printf("Reg\t%04x\t%02x\n", reg, in);

    ax5043WriteReg(reg, in);

    return PQWS_SUCCESS;
}

int ax5043_spi_write_16(ax5043_conf_t *conf, uint16_t reg, uint16_t in) {
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    //printf("Reg\t%04x\t%02x\n", reg, (in >> 8)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+1, (in >> 0)&0xFF);

    ax5043WriteReg2(reg, in);

    return PQWS_SUCCESS;
}

int ax5043_spi_write_24(ax5043_conf_t *conf, uint16_t reg, uint32_t in) {
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    //printf("Reg\t%04x\t%02x\n", reg, (in >> 16)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+1, (in >> 8)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+2, (in >> 0)&0xFF);

    ax5043WriteReg3(reg, in);

    return PQWS_SUCCESS;
}

int ax5043_spi_write_32(ax5043_conf_t *conf, uint16_t reg, uint32_t in) {
    if (!is_ax5043_conf_valid(conf)) {
        return -PQWS_INVALID_PARAM;
    }

    //printf("Reg\t%04x\t%02x\n", reg, (in >> 24)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+1, (in >> 16)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+2, (in >> 8)&0xFF);
    //printf("Reg\t%04x\t%02x\n", reg+3, (in >> 0)&0xFF);

    ax5043WriteReg4(reg, in);

    return PQWS_SUCCESS;
}

/**
 * Sets properly some undocumented TX registers
 * @param conf  the AX5043 configuration handler
 * @return 0 on success or appropriate negative error code
 */
static inline int set_tx_black_magic_regs(ax5043_conf_t *conf) {
    int ret;
    ret = ax5043_spi_write_8(conf, 0xF00, 0x0F);
    if (ret) {
        return ret;
    }

    ret = ax5043_spi_write_8(conf, 0xF0C, 0x0);
    if (ret) {
        return ret;
    }

    // Added by Jonathan Brandenburg
    ret = ax5043_spi_write_8(conf, 0xF0D, 0x03);
    if (ret) {
        return ret;
    }

    // Added by Jonathan Brandenburg
    ret = ax5043_spi_write_8(conf, 0xF10, 0x03);
    if (ret) {
        return ret;
    }

    // The following line is used for a TCXO
    //ret = ax5043_spi_write_8(conf, 0xF11, 0x0);
    // The following line is used for a crystal
    ret = ax5043_spi_write_8(conf, 0xF11, 0x07);
    if (ret) {
        return ret;
    }

    ret = ax5043_spi_write_8(conf, 0xF1C, 0x07);
    if (ret) {
        return ret;
    }

    ret = ax5043_spi_write_8(conf, 0xF44, 0x24);
    if (ret) {
        return ret;
    }

    /* Dafuq? Got it from RadioLab */
    ret = ax5043_spi_write_8(conf, 0xF18, 0x06);
    return ret;
}

/**
 * Enables/Disables the power amplifier pin
 * @param conf the AX5043 configuration handler
 * @param enable 1 to enable 0 to disable
 * @return 0 on success or appropriate negative error code
 */
int ax5043_enable_pwramp(ax5043_conf_t *conf, uint8_t enable) {
    int ret;
    ax5043_set_antsel(conf, enable);
    ret = ax5043_spi_write_8(conf, AX5043_REG_PWRAMP, ~enable & 0x1);

    if (ret) {
        usleep(PWRAMP_RAMP_PERIOD_US);
    }
    return ret;
}

int ax5043_set_antsel(ax5043_conf_t *conf, uint8_t val) {
    return ax5043_spi_write_8(conf, AX5043_REG_PINFUNCANTSEL, val & 0x1);
}

/**
 * Wait for the AX5043 to finish transmitting, putting new data in the FIFO as space becomes available
 * @return 0 on success, or appropriate negative error code
 */
int ax5043_wait_for_transmit() {
    if (!single_fifo_access) {
        while (__tx_active) {
            static int transmittedPostamble = 0;

            //usleep(100);
    
            int ret;
            uint8_t data_cmd[3] = { AX5043_FIFO_VARIABLE_DATA_CMD, 0, 0 };
            size_t avail;
            size_t chunk_size;
    
            if (!__ax5043_conf) {
                return -PQWS_INVALID_PARAM;
            }
    
            /* Determine if TX is done */
            uint8_t radiostate = 0;
            ret = ax5043_spi_read_8(__ax5043_conf, &radiostate,
            AX5043_REG_RADIOSTATE);
            if (ret) {
                return ret;
            }
            radiostate &= 0x0f;

            if (radiostate == 0) {
                /* tx is done */
                __tx_frame_end(__ax5043_conf);
                transmittedPostamble = 0;
                #ifdef DEBUG_LOGGING
                  printf("INFO: TX done\n");
                #endif
                return PQWS_SUCCESS;
            }
    
            /* Determine FIFO free space */
            uint16_t fifofree = 0;
            ret = ax5043_spi_read_16(__ax5043_conf, &fifofree,
            AX5043_REG_FIFOFREE1);
            if (ret) {
                return ret;
            }
    
            /* If FIFO has free space fill in data */
            if (fifofree > AX5043_FIFO_FREE_THR && (__tx_remaining || !transmittedPostamble)) {

                /* Always left some space for the postamble for a simplified logic */
                avail = min_ul(
                AX5043_FIFO_FREE_THR - sizeof(data_cmd) - sizeof(__postamble_cmd),
                        __tx_remaining);

                data_cmd[1] = (uint8_t) (avail + 1);
                chunk_size = sizeof(data_cmd) + avail;
                memcpy(__tx_fifo_chunk + sizeof(data_cmd), __tx_buf + __tx_buf_idx,
                        avail);

                if (avail == __tx_remaining) {
                    transmittedPostamble = 1;

                    data_cmd[2] = AX5043_FIFO_PKTEND;
                    memcpy(__tx_fifo_chunk + chunk_size, __postamble_cmd,
                            sizeof(__postamble_cmd));
                    chunk_size += sizeof(__postamble_cmd);
                }
                memcpy(__tx_fifo_chunk, data_cmd, sizeof(data_cmd));
                ax5043_spi_write(__ax5043_conf, AX5043_REG_FIFODATA,
                        __tx_fifo_chunk, (uint32_t) chunk_size);
                /* Commit to FIFO ! */
                ret = ax5043_spi_write_8(__ax5043_conf, AX5043_REG_FIFOSTAT,
                AX5043_FIFO_COMMIT_CMD);
    
                __tx_remaining -= (uint32_t) avail;
                __tx_buf_idx += avail;
    
            }
        }
    } else {
        while (__tx_active) {
            int ret;
            /* Determine if TX is done */
            uint8_t radiostate = 0;
            ret = ax5043_spi_read_8(__ax5043_conf, &radiostate,
            AX5043_REG_RADIOSTATE);
            if (ret) {
                return ret;
            }
            radiostate &= 0x0f;
            if (radiostate == 0) {
                /* tx is done */
                __tx_active = 0;
                #ifdef DEBUG_LOGGING
                  printf("INFO: TX done\n");
                #endif
            }
        }
    }

    return PQWS_SUCCESS;
}

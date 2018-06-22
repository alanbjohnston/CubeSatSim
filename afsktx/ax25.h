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

#ifndef AX25_H_
#define AX25_H_

#include <stdint.h>
#include "ax5043.h"

#define AX25_MAX_ADDR_LEN       28
#define AX25_MAX_FRAME_LEN      256
#define AX25_MIN_ADDR_LEN       14
#define AX25_SYNC_FLAG          0x7E
#define AX25_MIN_CTRL_LEN       1
#define AX25_MAX_CTRL_LEN       2
#define AX25_CALLSIGN_MAX_LEN   6
#define AX25_CALLSIGN_MIN_LEN   2
#define AX25_PREAMBLE_LEN       16
#define AX25_POSTAMBLE_LEN      16

/**
 * AX.25 Frame types
 */
typedef enum {
    AX25_I_FRAME, //!< AX25_I_FRAME Information frame
    AX25_S_FRAME, //!< AX25_S_FRAME Supervisory frame
    AX25_U_FRAME, //!< AX25_U_FRAME Unnumbered frame
    AX25_UI_FRAME /**!< AX25_UI_FRAME Unnumbered information frame */
} ax25_frame_type_t;

typedef struct {
    uint8_t preamble_len;
    uint8_t postable_len;
    uint8_t addr_field[AX25_MAX_ADDR_LEN];
    uint32_t addr_field_len;
} ax25_conf_t;

int
ax25_init(ax25_conf_t *conf, const uint8_t *dest_addr, uint8_t dest_ssid,
        const uint8_t *src_addr, uint8_t src_ssid, uint8_t preamble_len,
        uint8_t postamble_len);
int ax25_tx_frame(ax25_conf_t *hax25, ax5043_conf_t *hax,
        const uint8_t *payload, uint32_t len);

#endif /* AX25_H_ */

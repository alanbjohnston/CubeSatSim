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

//#include <curl/curl.h>
#include "ax25.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "ax5043.h"
#include "status.h"

static uint8_t __tx_buffer[MAX_FRAME_LEN];

/**
 * Creates the header field of the AX.25 frame
 * @param conf the AX.25 handle
 * @param dest_addr the destination callsign address
 * @param dest_ssid the destination SSID
 * @param src_addr the callsign of the source
 * @param src_ssid the source SSID
 * @param preamble_len the number of the AX.25 repetitions in the preamble
 * @param postamble_len the number of the AX.25 repetitions in the postamble
 */
int ax25_init(ax25_conf_t *conf, const uint8_t *dest_addr, uint8_t dest_ssid,
        const uint8_t *src_addr, uint8_t src_ssid, uint8_t preamble_len,
        uint8_t postamble_len) {
    uint16_t i = 0;

    if (!conf || !dest_addr || !src_addr || preamble_len < 4
            || !postamble_len) {
        return -PQWS_INVALID_PARAM;
    }

    conf->preamble_len = preamble_len;
    conf->postable_len = postamble_len;
    uint8_t *out = conf->addr_field;

    for (i = 0; i < strnlen((char *) dest_addr, AX25_CALLSIGN_MAX_LEN); i++) {
        *out++ = (uint8_t) (dest_addr[i] << 1);
    }
    /*
     * Perhaps the destination callsign was smaller that the maximum allowed.
     * In this case the leftover bytes should be filled with space
     */
    for (; i < AX25_CALLSIGN_MAX_LEN; i++) {
        *out++ = ' ' << 1;
    }
    /* Apply SSID, reserved and C bit */
    /* FIXME: C bit is set to 0 implicitly */
    *out++ = (uint8_t) ((0x0F & dest_ssid) << 1) | 0x60;
    //*out++ = ((0b1111 & dest_ssid) << 1) | 0b01100000;

    for (i = 0; i < strnlen((char *) src_addr, AX25_CALLSIGN_MAX_LEN); i++) {
        *out++ = (uint8_t) (src_addr[i] << 1);
    }
    for (; i < AX25_CALLSIGN_MAX_LEN; i++) {
        *out++ = ' ' << 1;
    }
    /* Apply SSID, reserved and C bit. As this is the last address field
     * the trailing bit is set to 1.
     */
    /* FIXME: C bit is set to 0 implicitly */
    *out++ = (uint8_t) ((0x0F & src_ssid) << 1) | 0x61;
    //*out++ = ((0b1111 & src_ssid) << 1) | 0b01100001;
    conf->addr_field_len = AX25_MIN_ADDR_LEN;
    return PQWS_SUCCESS;
}

int ax25_tx_frame(ax25_conf_t *hax25, ax5043_conf_t *hax,
        const uint8_t *payload, uint32_t len) {
    if (!hax25 || !hax || !payload || !len) {
        return -PQWS_INVALID_PARAM;
    }

    memcpy(__tx_buffer, hax25->addr_field, hax25->addr_field_len);
    memcpy(__tx_buffer + hax25->addr_field_len, payload, len);
   
#ifdef SATNOGS
    printf("\n");
    char post_data[1024];
    char hex_data[512];
    char hex_octet[4];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    memset(post_data,0,strlen(post_data));
    memset(hex_data,0,strlen(hex_data));
    //printf("1:%s\n",post_data);

    int jj;
    for(jj = 0; jj < 118; jj++) {
        sprintf(hex_octet, "%02x",__tx_buffer[jj]);
        strcat(hex_data, hex_octet); 
    }  // Note assumes EDT, change offset (+4) to UTC 
/*
  sprintf(post_data,"noradID=99999&source=KU2Y&timestamp=%d-%d-%dT%d:%d:%d.500Z&frame=%s&locator=longLat&longitude=75.3492W&latitude=40.0376N&&azimuth=360&elevation=90.0", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, (tm.tm_hour + 4) % 24, tm.tm_min, tm.tm_sec, hex_data);

        printf("curl post data: %s\n",post_data);

  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {
    //curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
    curl_easy_setopt(curl, CURLOPT_URL, "https://db.satnogs.org/api/telemetry/");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
 
     curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(post_data));

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "ERROR: AX25.C curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
 
    curl_easy_cleanup(curl); 
  }
  */
#endif        

    return ax5043_tx_frame(hax, __tx_buffer, len + hax25->addr_field_len,
            hax25->preamble_len, hax25->postable_len, 1000);
}

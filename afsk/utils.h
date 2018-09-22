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

#ifndef UTILS_H_
#define UTILS_H_

#include <sys/time.h>

#define BIT(x)   (1 << x)

static inline size_t min_ul(size_t x, size_t y) {
    size_t ret = x < y ? x : y;
    return ret;
}

/**
 *
 * @return the milliseconds from the start of the program.
 * Time is handled based on TIM2 timer.
 */
static inline uint32_t millis() {
    static struct timeval tv_start;
    static int tv_start_initialized = 0;
    if (!tv_start_initialized) {
        gettimeofday(&tv_start, NULL);
    }

    struct timeval tv_current;
    gettimeofday(&tv_current, NULL);

    uint32_t millis_elapsed = 0;
    millis_elapsed = (uint32_t) (tv_current.tv_sec - tv_start.tv_sec) * 1000;
    millis_elapsed += (uint32_t) (tv_current.tv_usec - tv_start.tv_usec) / 1000;
    return millis_elapsed;
}

#endif /* UTILS_H_ */

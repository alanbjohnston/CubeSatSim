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

#ifndef STATUS_H_
#define STATUS_H_

/**
 * Error codes of the PQWS. Users should return the negative of the error
 * codes. NO_ERROR is set to zero.
 */
typedef enum {
    PQWS_SUCCESS = 0,                    //!< All ok!
    PQWS_INVALID_PARAM,                   //!< An invalid parameter was given
    PQWS_MAX_SPI_TRANSFER_ERROR, //!< The requested SPI data transfer was larger than supported
    PQWS_NO_RF_FOUND,                     //!< No suitable RF chip found
    PQWS_AX5043_AUTORANGING_ERROR,        //!< Auto ranging failed on AX5043
    PQWS_TIMEOUT                          //!< A timeout occurred
} pqws_error_t;

#endif /* STATUS_H_ */

/*!
 \copyright
 Copyright (c) 2018 Brandenburg Tech, LLC
 All rights reserved.

 THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
 ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
 AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 Copyright (c) 2007,2008,2009,2010,2011,2012,2013, 2014 AXSEM AG
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     1.Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     2.Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     3.Neither the name of AXSEM AG, Duebendorf nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.
     4.All advertising materials mentioning features or use of this software
       must display the following acknowledgement:
       This product includes software developed by AXSEM AG and its contributors.
     5.The usage of this source code is only granted for operation with AX5043
       and AX8052F143. Porting to other radio or communication devices is
       strictly prohibited.

 THIS SOFTWARE IS PROVIDED BY AXSEM AG AND CONTRIBUTORS ``AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AXSEM AG AND CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 \file axradioinit_p.h
 \brief Provides an interface to initialze the AX5043 transceiver.
*/

#ifndef AX5043INIT_P_H_
#define AX5043INIT_P_H_

#include <stdint.h>

#define AXRADIO_ERR_NOERROR                     0x00 //!< Operation successful
#define AXRADIO_ERR_NOTSUPPORTED                0x01 //!< Operation not supported
#define AXRADIO_ERR_BUSY                        0x02 //!< Transceiver busy
#define AXRADIO_ERR_TIMEOUT                     0x03 //!< Operation timed out
#define AXRADIO_ERR_INVALID                     0x04 //!< Invalid parameter
#define AXRADIO_ERR_NOCHIP                      0x05 //!< Transceiver not found
#define AXRADIO_ERR_RANGING                     0x06 //!< Frequency could not be ranged
#define AXRADIO_ERR_LOCKLOST                    0x07 //!< Lost PLL lock
#define AXRADIO_ERR_RETRANSMISSION              0x08 //!< Retrasnmitted packet
#define AXRADIO_ERR_RESYNC                      0x09 //!< Restarts synchronization
#define AXRADIO_ERR_RESYNCTIMEOUT               0x0a //!< Synchronization timed out
#define AXRADIO_ERR_RECEIVESTART                0x0b //!< Receiver restarted

//! Structure containing a four byte X.25 address
struct axradio_address {
    uint8_t addr[4]; //!< Four byte X.25 address
};

/*! \fn uint8_t axradio_init(void)
    \brief Initialize the AX5043 radio transceiver.
    \return AXRADIO_ERR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t axradio_init(void);

/*! \fn uint8_t axradio_setfreq(int32_t f)
    \brief Set the receive and transmit frequency.
    \param f The frequency in Hertz.
    \return AXRADIO_ERR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t axradio_setfreq(int32_t f);

#endif /* AX5043INIT_P_H_ */

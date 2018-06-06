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

 \file axradiomode_p.h
 \brief Provides an interface to change the transceiver mode.
*/

#ifndef AX5043MODE_P_H_
#define AX5043MODE_P_H_

#include <stdint.h>

/*! \fn uint8_t mode_tx()
    \brief Switch the tranceiver into transmit mode.
    \return AXRADIO_ERROR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t mode_tx(void);

/*! \fn uint8_t mode_rx()
    \brief Switch the tranceiver into receive mode.

    The receive buffer may contain garbage and reading from the buffer
    will obtain and discard that garbage.
    \return AXRADIO_ERROR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t mode_rx(void);

#endif /* AX5043MODE_P_H_ */

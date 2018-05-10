/*! \file ax5043mode_p.h
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

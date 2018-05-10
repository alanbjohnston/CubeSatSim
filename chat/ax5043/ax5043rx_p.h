/*! \file ax5043rx_p.h
    \brief Provides an interface to receive packets using the digital transceiver.
*/
#ifndef AX5043RX_P_H_
#define AX5043RX_P_H_

#include <stdint.h>

/*! \fn uint8_t receive_packet(void)
    \brief Receive a packet from the digital transceiver receive buffer.
    \return  AXRADIO_ERROR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t receive_packet(void);

#endif /* AX5043RX_P_H_ */

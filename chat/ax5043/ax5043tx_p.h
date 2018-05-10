/*! \file ax5043tx_p.h
    \brief Provides an interface to transmit packets using the digital transceiver.
*/
#ifndef AX5043TX_P_H_
#define AX5043TX_P_H_

#include <stdint.h>

#include "ax5043init_p.h"

/*! \fn uint8_t transmit_packet(const struct axradio_address *addr, const uint8_t *pkt, uint16_t pktlen)
    \brief Transmit a packet using the digital transceiver
    \param addr The address of the desired destiantion radio, if used.
    \param pkt The byte data to be transmitted.
    \param pktlen The number of bytes in pkt to be transmitted.
    \return AXRADIO_ERROR_NOERROR on success, otherwise a value indicating an error.
    \sa AXRADIO_ERR_NOERROR
*/
uint8_t transmit_packet(const struct axradio_address *addr, const uint8_t *pkt, uint16_t pktlen);

#endif /* AX5043TX_P_H_ */

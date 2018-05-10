/*! \file ax5043init_p.h
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

#endif /* AX5043INIT_P_H_ */

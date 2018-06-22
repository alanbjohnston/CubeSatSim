/*! \copyright
 Copyright (c) 2018 Brandenburg Tech, LLC
 All right reserved.
 ---
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

 \file ax5043spi_p.h
 \brief Provides an abstraction layer for the SPI interface communicating to the digital transceiver
 */

#ifndef AX5043SPI_P_H_
#define AX5043SPI_P_H_

#include <stdint.h>

#define SPI_CHANNEL (0) //!< The default SPI channel for the digital transceiver
#define SPI_SPEED (32000000) //!< The default SPI bus speed for the digital transceiver

/*! \fn void setSpiChannel(int newSpiChannel)
 \brief Set the SPI channel for the digital transceiver.

 setSpiChannel must be called before initializeSpi(). The default is SPI_CHANNEL.
 \param newSpiChannel The SPI channel for the digital transceiver.
 \sa SPI_CHANNEL
 \sa initializeSpi
 */
void setSpiChannel(int newSpiChannel);

/*! \fn void setSpiSpeed(int newSpiSpeed)
 \brief Set the SPI bus speed for the digital transceiver.


 setSpiSpeed must be called before initializeSpi(). The default is SPI_SPEED.
 \param newSpiSpeed The SPI bus speed for the digital transceiver.
 \sa SPI_SPEED
 \sa initializeSpi
 */
void setSpiSpeed(int newSpiSpeed);

/*! \fn void initializeSpi()
 \brief Initilize the SPI bus to communicate with the digital transceiver.

 setSpiChannel() and setSpiSpeed() must both be called before initializeSPI().
 \sa setSpiChannel
 \sa setSpiSpeed
 */
void initializeSpi(void);

/*! \fn void ax5043WriteReg(uint16_t reg, uint8_t val)
 \brief Write a value to an AX5043 register.
 \param reg The register to write.
 \param val The value to right to the register.
 */
void ax5043WriteReg(uint16_t reg, uint8_t val);

/*! \fn void ax5043WriteReg2(uint16_t reg, uint16_t val)
 \brief Write a two byte value to an AX5043 register.
 \param reg The register to write.
 \param val The value to right to the register.
 */
void ax5043WriteReg2(uint16_t reg, uint16_t val);

/*! \fn void ax5043WriteReg3(uint16_t reg, uint32_t val)
 \brief Write a three byte value to an AX5043 register.
 \param reg The register to write.
 \param val The value to right to the register.
 */
void ax5043WriteReg3(uint16_t reg, uint32_t val);

/*! \fn void ax5043WriteReg4(uint16_t reg, uint32_t val)
 \brief Write a four byte value to an AX5043 register.
 \param reg The register to write.
 \param val The value to right to the register.
 */
void ax5043WriteReg4(uint16_t reg, uint32_t val);

/*! \fn void ax5043WriteRegN(uint16_t reg, const uint8_t *in, uint32_t len)
 \brief Write a four byte value to an AX5043 register.
 \param reg The register to write.
 \param val The value to right to the register.
 */
void ax5043WriteRegN(uint16_t reg, const uint8_t *in, uint32_t len);

/*! \fn uint8_t ax5043ReadReg(uint16_t reg)
 \brief Read a one byte value from an AX5043 register.
 \param reg The register to read.
 \return The value read from the register.
 */
uint8_t ax5043ReadReg(uint16_t reg);

/*! \fn uint16_t ax5043ReadReg2(uint16_t reg)
 \brief Read a two value from an AX5043 register.
 \param reg The register to read.
 \return The value read from the register.
 */
uint16_t ax5043ReadReg2(uint16_t reg);

/*! \fn uint32_t ax5043ReadReg3(uint16_t reg)
 \brief Read a three byte value from an AX5043 register.
 \param reg The register to read.
 \return The value read from the register.
 */
uint32_t ax5043ReadReg3(uint16_t reg);

/*! \fn uint32_t ax5043ReadReg4(uint16_t reg)
 \brief Read a four byte value from an AX5043 register.
 \param reg The register to read.
 \return The value read from the register.
 */
uint32_t ax5043ReadReg4(uint16_t reg);

#endif /* AX5043SPI_P_H_ */

/**
 * File is updated with lib-hal API's.
 */
/**
 @file MCU.h
 */
/*
 * ============================================================================
 * History
 * =======
 * Nov 2019		Initial beta for FT81x and FT80x
 * Mar 2020		Updated beta - added BT815/6 commands
 * Mar 2021		Beta with BT817/8 support added
 *
 *
 *
 *
 *
 * (C) Copyright,  Bridgetek Pte. Ltd.
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * http://www.ftdichip.com/FTSourceCodeLicenceTerms.htm ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

#ifndef MCU_HEADER_H
#define	MCU_HEADER_H

#include <cstdint>

#include "hal_gpio.h"
#include "hal_spi.h"

/**
 @brief MCU specific initialisation
 @details Must contain any MCU-specific initialisation. This will typically be
 	 setting up the SPI bus, GPIOs and operating environment requirements.
 */
void MCU_Init(void);

/**
 @brief MCU specific setup
 @details Called after the EVE has been power cycled and started. Contains
 	 any MCU-specific configuration options for the EVE.
 */
void MCU_Setup(void);

/**
 @brief MCU specific chip select enable
 @details This function will pull the chip select line to the EVE low to
 	 allow data transmission on the SPI bus.
 	 The EVE requires chip select to toggle frequently.
 */
inline void MCU_CSlow() {
	FUNC_PREFIX(gpio_clr(FT8XX_LCD_CS_GPIO));
}

/**
 @brief MCU specific chip select deassert
 @details This function will pull the chip select line to the EVE high to
 	 prevent data transmission on the SPI bus.
 	 The EVE requires chip select to toggle frequently.
 */
inline void MCU_CShigh() {
	FUNC_PREFIX(gpio_set(FT8XX_LCD_CS_GPIO));
}

/**
 @brief MCU specific power down enable
 @details This function will pull the power down line to the EVE low to
 	 force the device into power down mode.
 	 This will be done during EVE initialisation and can be done to allow
 	 deep power saving.
 */
inline void MCU_PDlow() {
	FUNC_PREFIX(gpio_clr(FT8XX_LCD_DC_GPIO));
}

/**
 @brief MCU specific power down disable
 @details This function will pull the power down line to the EVE high to
 	 enable normal operation of the EVE.
 	 This will be done during EVE initialisation and can be done to allow
 	 recovery from deep power saving.
 */
inline void MCU_PDhigh() {
	FUNC_PREFIX(gpio_set(FT8XX_LCD_DC_GPIO));
}

/**
 @brief MCU specific SPI write
 @details Performs an SPI write of the data block and discards the data
 	 received in response.
 @param DataToWrite - pointer to buffer to write.
 @param length - number of bytes to write.
 */
inline void MCU_SPIWrite(const uint8_t *pDataToWrite, const uint32_t nLength) {
	FUNC_PREFIX(spi_writenb(reinterpret_cast<const char *>(pDataToWrite), nLength));
}

/**
 @brief MCU specific SPI 8 bit read
 @details Performs an SPI dummy write and returns the data received in
    	 response.
 @returns Data received from EVE.
 */
inline uint8_t MCU_SPIRead8() {
	char buffer[1];

	FUNC_PREFIX(spi_transfern(buffer, 1));

	return buffer[0];
}

/**
 @brief MCU specific SPI 8 bit write
 @details Performs an SPI write and discards the data received in
    	 response.
 @param Data to write to EVE.
 */
inline void MCU_SPIWrite8(const uint8_t DataToWrite) {
	char buffer[1];

	buffer[0] = DataToWrite;

	FUNC_PREFIX(spi_writenb(buffer, 1));
}

/**
 @brief MCU specific SPI 16 bit read
 @details Performs an SPI dummy write and returns the data received in
    	 response.
 @returns Data received from EVE.
 */
inline uint16_t MCU_SPIRead16() {
	char buffer[2];

	FUNC_PREFIX(spi_transfern(buffer, 2));

	return static_cast<uint16_t>((buffer[0] << 8) | buffer[1]);
}

/**
 @brief MCU specific SPI 16 bit write
 @details Performs an SPI write and discards the data received in
    	 response.
 @param Data to write to EVE.
 */
inline void MCU_SPIWrite16(const uint16_t DataToWrite) {
	FUNC_PREFIX(spi_write(DataToWrite));
}

/**
 @brief MCU specific SPI 24 bit read
 @details Performs an SPI dummy write and returns the data received in
    	 response.
 @returns Data received from EVE.
 */
uint32_t MCU_SPIRead24(void);

/**
 @brief MCU specific SPI 24 bit write
 @details Performs an SPI write and discards the data received in
    	 response.
 @param Data to write to EVE.
 */
inline void MCU_SPIWrite24(const uint32_t DataToWrite) {
	char buffer[3];

	buffer[0] = static_cast<uint8_t>(DataToWrite >> 24);
	buffer[1] = static_cast<uint8_t>(DataToWrite >> 16);
	buffer[2] = static_cast<uint8_t>(DataToWrite >>  8);

	FUNC_PREFIX(spi_writenb(buffer, 3));
}

/**
 @brief MCU specific SPI 32 bit read
 @details Performs an SPI dummy write and returns the data received in
    	 response.
 @returns Data received from EVE.
 */
inline uint32_t MCU_SPIRead32() {
	char buffer[4];

	FUNC_PREFIX(spi_transfern(buffer, 4));

	return static_cast<uint32_t>((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3]);
}

/**
 @brief MCU specific SPI 32 bit write
 @details Performs an SPI write and discards the data received in
    	 response.
 @param Data to write to EVE.
 */
inline void MCU_SPIWrite32(const uint32_t DataToWrite) {
	char buffer[4];

	buffer[0] = static_cast<uint8_t>(DataToWrite >> 24);
	buffer[1] = static_cast<uint8_t>(DataToWrite >> 16);
	buffer[2] = static_cast<uint8_t>(DataToWrite >>  8);
	buffer[3] = DataToWrite & 0xFF;

	FUNC_PREFIX(spi_writenb(buffer, 4));
}

/**
 @brief MCU specific 20 ms delay
 @details Cause the MCU to idle or otherwise delay for a minimum of
 	 20 milliseconds. This is used during initialisation to perform a
 	 power down of the EVE for a controlled minimum period of time.
 */
inline void MCU_Delay_20ms() {
	udelay(20 * 1000);
}

/**
 @brief MCU specific 500 ms delay
 @details Cause the MCU to idle or otherwise delay for a minimum of
 	 500 milliseconds. This is used during initialisation to perform a
 	 power down of the EVE for a controlled minimum period of time.
 */
inline void MCU_Delay_500ms() {
	udelay(500 * 1000);
}

/**
 @brief MCU specific byte swapping routines
 @details EVE addresses from the HAL_SetReadAddress and HAL_SetWriteAddress
 	 are sent in big-endian format. However, data for registers or memory
 	 mapped areas are in little-endian format.
 */
//@{
inline uint16_t MCU_htobe16(const uint16_t h) {
	return h;
}

inline uint32_t MCU_htobe32(const uint32_t h) {
	return h;
}

inline uint16_t MCU_htole16(const uint16_t h) {
	return __builtin_bswap16(h);
}

inline uint32_t MCU_htole32(const uint32_t h) {
	return __builtin_bswap32(h);
}

inline uint16_t MCU_be16toh(const uint16_t h) {
	return h;
}

inline uint32_t MCU_be32toh(const uint32_t h) {
	return h;
}

inline uint16_t MCU_le16toh(const uint16_t h) {
	return __builtin_bswap16(h);
}

inline uint32_t MCU_le32toh(uint32_t h) {
	return __builtin_bswap32(h);
}
//@}

#endif	/* MCU_HEADER_H */

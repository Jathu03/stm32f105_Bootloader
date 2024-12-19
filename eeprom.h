#ifndef EEPROM_H
#define EEPROM_H

#include "spi.h"

// EEPROM-specific commands
#define EEPROM_CMD_READ  0x03
#define EEPROM_CMD_WRITE 0x02
#define EEPROM_CMD_WREN  0x06
#define EEPROM_CMD_RDSR  0x05
#define EEPROM_CMD_WRSR  0x01

#define EEPROM_PAGE_SIZE 256
#define EEPROM_SIZE      (256 * 1024)  // 256 KB
#define EEPROM_CS_PIN    GPIO_PIN_4
#define EEPROM_CS_PORT   GPIOA

void EEPROM_Init(void);
void EEPROM_WriteEnable(void);
HAL_StatusTypeDef EEPROM_Read(uint32_t addr, uint8_t *data, uint16_t size);
HAL_StatusTypeDef EEPROM_Write(uint32_t addr, uint8_t *data, uint16_t size);

#endif // EEPROM_H

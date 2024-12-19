#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "eeprom.h"

#define APPLICATION_START_ADDR 0x08004000
#define EEPROM_FIRMWARE_ADDR   0x00000000  // Starting address of firmware in EEPROM

void Bootloader_Run(void);
void Bootloader_JumpToApplication(void);
HAL_StatusTypeDef Bootloader_UpdateFirmware(void);

#endif // BOOTLOADER_H

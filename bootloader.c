#include "bootloader.h"
#include "stm32f4xx_hal.h"

void Bootloader_Run(void) {
    uint8_t firmware_data[EEPROM_PAGE_SIZE];
    uint32_t flash_addr = APPLICATION_START_ADDR;
    uint32_t eeprom_addr = EEPROM_FIRMWARE_ADDR;

    // Erase application area
    FLASH_EraseInitTypeDef erase_config;
    uint32_t page_error;
    erase_config.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_config.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase_config.Sector = FLASH_SECTOR_1;  // Adjust sector
    erase_config.NbSectors = 1;

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&erase_config, &page_error);

    // Write firmware to flash
    while (eeprom_addr < EEPROM_SIZE) {
        if (EEPROM_Read(eeprom_addr, firmware_data, EEPROM_PAGE_SIZE) != HAL_OK) break;

        for (uint32_t i = 0; i < EEPROM_PAGE_SIZE; i += 4) {
            uint32_t word = *(uint32_t *)&firmware_data[i];
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addr, word) != HAL_OK) break;

            flash_addr += 4;
        }

        eeprom_addr += EEPROM_PAGE_SIZE;
    }

    HAL_FLASH_Lock();

    Bootloader_JumpToApplication();
}

void Bootloader_JumpToApplication(void) {
    void (*app_reset_handler)(void) = (void *)(*(volatile uint32_t *)(APPLICATION_START_ADDR + 4));

    __set_MSP(*(volatile uint32_t *)APPLICATION_START_ADDR);
    app_reset_handler();
}

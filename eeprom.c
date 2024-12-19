#include "eeprom.h"

void EEPROM_Init(void) {
    HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_SET);  // Deselect EEPROM
}

void EEPROM_WriteEnable(void) {
    uint8_t cmd = EEPROM_CMD_WREN;
    HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_SET);
}

HAL_StatusTypeDef EEPROM_Read(uint32_t addr, uint8_t *data, uint16_t size) {
    uint8_t cmd[4] = {
        EEPROM_CMD_READ,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };

    HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
    HAL_StatusTypeDef status = HAL_SPI_Receive(&hspi1, data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_SET);

    return status;
}

HAL_StatusTypeDef EEPROM_Write(uint32_t addr, uint8_t *data, uint16_t size) {
    while (size > 0) {
        uint16_t chunk_size = (size > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : size;
        uint8_t cmd[4] = {
            EEPROM_CMD_WRITE,
            (uint8_t)(addr >> 16),
            (uint8_t)(addr >> 8),
            (uint8_t)(addr)
        };

        EEPROM_WriteEnable();

        HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY);
        HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, data, chunk_size, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(EEPROM_CS_PORT, EEPROM_CS_PIN, GPIO_PIN_SET);

        if (status != HAL_OK) return status;

        addr += chunk_size;
        data += chunk_size;
        size -= chunk_size;

        HAL_Delay(5);  // Wait for write operation to complete
    }

    return HAL_OK;
}

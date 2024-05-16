/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#pragma once

#include <project.h>

int32 ReadSensorTemperature();
int32 ReadSensorHumidity();
int32 ReadSensorCO();
int32 ReadSensorCO2();
int32 ReadSensorCH4();
int32 ReadSensorO2();

int initializeSensors();

uint8 readReg16(uint8 addr, uint16 reg, uint16* val);
uint8 readReg16crc(uint8 addr, uint16 reg, uint16* val);
uint8 writeReg16(uint8 addr, uint16 reg, uint16 val);

uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count);

#define TIMEOUT 20

#define SCD41_ADDR 0x62

#define REG_Start 0x21B1
#define REG_Measurement 0xEC05

/* [] END OF FILE */

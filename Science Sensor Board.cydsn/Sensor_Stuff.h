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
int32 ReadSensorO2(uint8 reg, uint16* val);

#define DEVICE_ADDR 0x73


/* [] END OF FILE */

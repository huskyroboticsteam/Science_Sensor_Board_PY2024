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
#include <project.h>
#include "main.h"
#include "cyapicallbacks.h"
#include "Sensor_Stuff.h"

int32 ReadSensorTemperature() {
    // TODO
    return 0;
}

int32 ReadSensorHumidity() {
    // TODO
    return 0;
}

int32 ReadSensorCO() {
    // TODO
    return 0;
}

int32 ReadSensorCO2() {
    // TODO -> I2C Version
    uint32 val;
    readReg24(SCD41_ADDR, REG_Measurement, &val);
    return val;
}

int32 ReadSensorCH4() {
    // TODO
    return 0;
}

int32 ReadSensorO2() {
    // TODO
    return 0;
}

int initializeSensors() {
    writeReg16(SCD41_ADDR, REG_Start, 0); //Starting periodic sensor for CO2
    return 0;
}

// read 16 bytes from a 16 bit address
uint8 readReg16(uint8 addr, uint16 reg, uint32* val) {
    uint8 b1, b2;
    I2C_I2CMasterClearStatus(); //clear the garbage

	I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	I2C_I2CMasterWriteByte(reg, TIMEOUT);
	I2C_I2CMasterSendStop(TIMEOUT);
	
	I2C_I2CMasterSendStart(addr, I2C_I2C_READ_XFER_MODE, TIMEOUT);
	I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &b2, TIMEOUT);
    I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, &b1, TIMEOUT);
    
    int err = I2C_I2CMasterSendStop(TIMEOUT);
    *val = ((uint16) b2 << 8) | b1;
	return err;
}

// read 24 bytes from a 16 bit address
uint8 readReg24(uint8 addr, uint16 reg, uint32* val) {
    uint8 b1, b2, b3;
    I2C_I2CMasterClearStatus(); //clear the garbage

	I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	I2C_I2CMasterWriteByte(reg >> 8, TIMEOUT);
    I2C_I2CMasterWriteByte(reg, TIMEOUT);
	// I2C_I2CMasterSendStop(TIMEOUT);
	
	I2C_I2CMasterSendStart(addr, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &b3, TIMEOUT);
	I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &b2, TIMEOUT);
    I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, &b1, TIMEOUT);
    
    int err = I2C_I2CMasterSendStop(TIMEOUT);
    *val = ((uint16) b3 << 16) |((uint16) b2 << 8) | b1;
	return err;
}

uint8 writeReg16(uint8 addr, uint16 reg, uint32 val) {
    uint8 b1, b2;
    b1 = val & 0xFF;
    b2 = val >> 8;
    chk = 
    I2C_I2CMasterClearStatus(); //clear the garbage
    
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	I2C_I2CMasterWriteByte(reg >> 8, TIMEOUT);
    I2C_I2CMasterWriteByte(reg & 0xFF, TIMEOUT);
    
    I2C_I2CMasterWriteByte(b2, TIMEOUT);
    I2C_I2CMasterWriteByte(b1, TIMEOUT);
    I2C_I2CMasterWriteByte(chk, TIMEOUT);
    
    return I2C_I2CMasterSendStop(TIMEOUT);
}
/* [] END OF FILE */

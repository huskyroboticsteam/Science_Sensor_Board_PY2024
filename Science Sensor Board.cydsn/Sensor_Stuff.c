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

char txData[TX_DATA_SIZE];

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
    uint16 val;
    uint32 err = readReg16crc(SCD41_ADDR, REG_Measurement, &val);
    if (err) {
        Print("Failed to read: ");
        PrintInt(err);
        Print("\r\n");
        return 0xFFFFFFFF;
    }
    return (int32) val;
}

int32 ReadSensorCH4() {
    // TODO
    return 0;
}

int32 ReadSensorO2() {
    // TODO
    return 0;
}

uint32 initializeSensors() {
    return writeReg0(SCD41_ADDR, REG_Start); //Starting periodic sensor for CO2
}

// read 16 bytes from a 16 bit address
uint32 readReg16(uint8 addr, uint16 reg, uint16* val) {
    uint32 err;
    uint8 data[2];
    I2C_I2CMasterClearStatus(); //clear the garbage

	err = I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    if (err) {
        I2C_I2CMasterSendStop(TIMEOUT);
        return err;
    }
	I2C_I2CMasterWriteByte(reg, TIMEOUT);
	I2C_I2CMasterSendStop(TIMEOUT);
	
	err = I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    if (err) {
        I2C_I2CMasterSendStop(TIMEOUT);
        return err;
    }
	I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, data, TIMEOUT);
    I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, data+1, TIMEOUT);
    I2C_I2CMasterSendStop(TIMEOUT);
    
    *val = ((uint16) data[0] << 8) | data[1];
	return 0;
}

// read 16 bytes from a 16 bit address, with crc
uint32 readReg16crc(uint8 addr, uint16 reg, uint16* val) {
    uint8 data[3];
    uint32 err;
    I2C_I2CMasterClearStatus(); //clear the garbage
    
    I2C_I2C_MSTR_ERR_LB_NAK;
    I2C_I2C_MSTR_NOT_READY;
    
	err = I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    if (err) {
        I2C_I2CMasterSendStop(TIMEOUT);
        return err;
    }
	I2C_I2CMasterWriteByte(reg >> 8, TIMEOUT);
    I2C_I2CMasterWriteByte(reg & 0xFF, TIMEOUT);
	// I2C_I2CMasterSendStop(TIMEOUT);
    // CyDelay(5);
	
	err = I2C_I2CMasterSendRestart(addr, I2C_I2C_READ_XFER_MODE, TIMEOUT);
    if (err) {
        I2C_I2CMasterSendStop(TIMEOUT);
        return err;
    }
    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, data, TIMEOUT);
	I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, data+1, TIMEOUT);
    I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, data+2, TIMEOUT);
    I2C_I2CMasterSendStop(TIMEOUT);
    
    uint8 crc = sensirion_common_generate_crc(data, 2);
    if (crc != data[2]) {
        Print("CRC mismatch, expected ");
        PrintInt(crc);
        Print(" got ");
        PrintInt(data[2]);
        Print("\r\n");
    }
    
    *val = ((uint16) data[0] << 8) | data[1];
	return 0;
}

// write no bytes to a register
uint32 writeReg0(uint8 addr, uint16 reg) {
    I2C_I2CMasterClearStatus(); //clear the garbage
    
    uint32 err = I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
    if (err) {
        I2C_I2CMasterSendStop(TIMEOUT);
        return err;
    }
    I2C_I2CMasterWriteByte(reg >> 8, TIMEOUT);
    I2C_I2CMasterWriteByte(reg & 0xFF, TIMEOUT);
    I2C_I2CMasterSendStop(TIMEOUT);
    return 0;
}

uint32 writeReg16(uint8 addr, uint16 reg, uint16 val) {
    uint8 b1, b2;
    b1 = val & 0xFF;
    b2 = val >> 8;
    
    uint8_t data[2] = {b1, b2};
    uint8_t chk = sensirion_common_generate_crc(data, 2);
    I2C_I2CMasterClearStatus(); //clear the garbage
    
    I2C_I2CMasterSendStart(addr, I2C_I2C_WRITE_XFER_MODE, TIMEOUT);
	I2C_I2CMasterWriteByte(reg >> 8, TIMEOUT);
    I2C_I2CMasterWriteByte(reg & 0xFF, TIMEOUT);
    
    I2C_I2CMasterWriteByte(b2, TIMEOUT);
    I2C_I2CMasterWriteByte(b1, TIMEOUT);
    I2C_I2CMasterWriteByte(chk, TIMEOUT);
    
    return I2C_I2CMasterSendStop(TIMEOUT);
}

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xff
uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count) {
    uint16_t current_byte;
    uint8_t crc = CRC8_INIT;
    uint8_t crc_bit;
    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte) {
    crc ^= (data[current_byte]);
    for (crc_bit = 8; crc_bit > 0; --crc_bit) {
        if (crc & 0x80)
            crc = (crc << 1) ^ CRC8_POLYNOMIAL;
        else
            crc = (crc << 1);
        }
    }
    return crc;
}
/* [] END OF FILE */

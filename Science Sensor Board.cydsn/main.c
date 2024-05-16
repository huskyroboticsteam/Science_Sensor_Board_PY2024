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

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "cyapicallbacks.h"
#include "CAN_Stuff.h"
#include "Sensor_Stuff.h"
#include "./HindsightCAN/CANLibrary.h"

// LED stuff
volatile uint8_t CAN_time_LED = 0;
volatile uint8_t ERROR_time_LED = 0;

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket can_recieve;
CANPacket can_send;
uint8 address = 0;

CY_ISR(Period_Reset_Handler) {
    CAN_time_LED++;
    ERROR_time_LED++;

    if (ERROR_time_LED >= 3) {
        LED_ERR_Write(OFF);
    }
    if (CAN_time_LED >= 3) {
        LED_CAN_Write(OFF);
    }
}

int main(void)
{ 
    Initialize();
    
    int err;
    
    uint16 temp;
    readReg16crc(SCD41_ADDR, 0x3682, &temp);
    PrintInt(temp);
    Print("/r/n");
    
    for(;;)
    {
        err = 0;
        
        if (!PollAndReceiveCANPacket(&can_recieve)) {
            LED_CAN_Write(ON);
            CAN_time_LED = 0;
            err = ProcessCAN(&can_recieve, &can_send);
        }
        
        if (err) DisplayErrorCode(err);
        
        if (DBG_UART_SpiUartGetRxBufferSize()) {
            DebugPrint(DBG_UART_UartGetByte());
        }
        
        CyDelay(10);
    }
}

void Initialize(void) {
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    
    address = 0; // TODO replace with science sensor address
    
    DBG_UART_Start();
    sprintf(txData, "Address: %x \r\n", address);
    Print(txData);
    
    LED_DBG_Write(0);
    
    InitCAN(DEVICE_GROUP_SCIENCE, (int)address);
    Timer_Period_Reset_Start();

    isr_Period_Reset_StartEx(Period_Reset_Handler);
    initializeSensors();
}

void DebugPrint(char input) {
    switch(input) {
        case 't':
            sprintf(txData, "Temp: %li", ReadSensorTemperature());
            break;
        case 'h':
            sprintf(txData, "Moist: %li", ReadSensorHumidity());
            break;
        case 'c':
            sprintf(txData, "CO: %li", ReadSensorCO());
            break;
        case 'd':
            sprintf(txData, "CO2: %li", ReadSensorCO2());
            break;
        case 'o':
            sprintf(txData, "O2: %li", ReadSensorO2());
            break;
        case 'm':
            sprintf(txData, "CH4: %li", ReadSensorCH4());
            break;
        default:
            sprintf(txData, "what");
            break;
    }
    Print(txData);
    Print("\r\n");
}

void DisplayErrorCode(uint8_t code) {    
    ERROR_time_LED = 0;
    LED_ERR_Write(ON);
    
    sprintf(txData, "Error %X\r\n", code);
    Print(txData);

    switch(code)
    {
        case ERROR_INVALID_TTC:
            Print("Cannot Send That Data Type!\n\r");
            break;
        default:
            //some error
            break;
    }
}

/* [] END OF FILE */

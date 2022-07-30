/**
 * @file project.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

void task1(void *pvParameters);
void task2(void *pvParameters);

volatile uint32_t ui32Loop;

int main(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0);
    
    if (xTaskCreate((TaskFunction_t)task1, "task1", configMINIMAL_STACK_SIZE, &ucParameterToPass, 5, &xHandle ) != pdPASS)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++){}
    }
    else
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++){}
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++){}
    }

    if (xTaskCreate((TaskFunction_t)task2, "task2", configMINIMAL_STACK_SIZE, NULL, 5, NULL ) != pdPASS)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++){}
    }
    else
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++){}
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++){}
    }
    vTaskStartScheduler();
    while (1){}
}


void task1(void *pvParameters) 
{
    while (1){
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        for(ui32Loop = 0; ui32Loop < 20000; ui32Loop++){}
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
        for(ui32Loop = 0; ui32Loop < 20000; ui32Loop++){}
    }
}

void task2(void *pvParameters)
{
    while(1){
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        for(ui32Loop = 0; ui32Loop < 2000; ui32Loop++){}
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        for(ui32Loop = 0; ui32Loop < 2000; ui32Loop++){}
    }
}
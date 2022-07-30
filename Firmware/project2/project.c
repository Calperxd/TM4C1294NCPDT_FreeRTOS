/**
 * @file project.c
 * @author gcalperseabra@gmail.com
 * @brief The aim is blink two leds one led flases every second while the other flash every 200ms
 * @version 0.1
 * @date 2022-07-29
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

// toggle the led 1 every second
void taskLed1(void *pvParameters)
{
    while (1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// toggle the led 2 every 200 second
void taskLed2(void *pvParameters)
{
    while (1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(100));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setupLed(void)
{
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0);
}


int main(void)
{
    setupLed();
    xTaskCreate((TaskFunction_t)taskLed1, "Led1", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
    xTaskCreate((TaskFunction_t)taskLed2, "Led2", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
    vTaskStartScheduler();
    while (1){}
}
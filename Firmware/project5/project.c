/**
 * @file project.c
 * @author Gabriel Calper Seabra - gcalperseabra@gmail.com
 * @brief There are two tasks in this project. Task 1 flashes the LED at ort PE12 every
second. Task 2 flashes the LED at port PE15 every 200 ms as in the previous project. Task 2 
is deleted by Task 1 after Task 1 makes 10 flashes. After this point only Task 1 remains in the
system and runs
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

static TaskHandle_t task1 = NULL;
static uint8_t Led1 = 0;


// toggle the led 1 every second
void taskLed1(void *pvParameters)
{
    TickType_t firstTime;
    const TickType_t interval = pdMS_TO_TICKS(500);
    firstTime = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&firstTime, interval);
        Led1 ^= 1UL << 1;
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, Led1);
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
    xTaskCreate((TaskFunction_t)taskLed1, "Led1", configMINIMAL_STACK_SIZE, NULL, 5, &task1 );
    vTaskStartScheduler();
    while (1){}
}
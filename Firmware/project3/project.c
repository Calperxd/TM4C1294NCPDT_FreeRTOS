/**
 * @file project.c
 * @author Gabriel Calper Seabra - gcalperseabra@gmail.com
 * @brief There are two tasks in this project. Task 1 flashes the LED at port PE12 every
second. Task 2 flashes the LED at port PE15 every 200 ms as in the previous project. Task 2 is sus-
pended by Task 1 after Task 1 makes 10 flashes, and as a result Task 2 stops flashing the LED. Task
2 is then resumed after Task 1 makes 15 flashes at which point Task 2 LED starts to flash again
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

volatile uint8_t countLed1 = 0;
static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;


// toggle the led 1 every second
void taskLed1(void *pvParameters)
{
    while (1)
    {
        if (countLed1 == 10) { vTaskSuspend(task2); }
        if (countLed1 == 15) 
        { 
            vTaskResume(task2); 
            countLed1 = 0; 
        }
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        vTaskDelay(pdMS_TO_TICKS(500));

        countLed1++;
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
    xTaskCreate((TaskFunction_t)taskLed1, "Led1", configMINIMAL_STACK_SIZE, NULL, 5, &task1 );
    xTaskCreate((TaskFunction_t)taskLed2, "Led2", configMINIMAL_STACK_SIZE, NULL, 5, &task2 );
    vTaskStartScheduler();
    while (1){}
}
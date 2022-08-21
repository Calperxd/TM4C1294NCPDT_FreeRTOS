/**
 * @file project.c
 * @author Gabriel Calper Seabra - gcalperseabra@gmail.com
 * @brief In this project there are 3 threads.
 * Thread 1 turn on the LED continuously
 * Thread 2 turn off the LED continuously
 * Thread 3 poll a button and change the threads priority as follow:
 * Task 1 has higher priority than Task 2
 * Task 1 and Task 2 have the same priorities
 * Task 1 has lower priority than Task 2
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdbool.h>

/* TivaWare includes */
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"



static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;
static TaskHandle_t task3 = NULL;

// turn on
void Task1(void *pvParameters)
{
    while (1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
    }
}
// turn off
void Task2(void *pvParameters)
{
    while (1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    }
}

//  polling
void Task3(void *pvParameters)
{
    TickType_t firstTime;
    const TickType_t interval = pdMS_TO_TICKS(10);
    firstTime = xTaskGetTickCount();
    uint8_t pin = 1;
    uint8_t option = 1;
    while (1)
    {
        pin = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(150));     // software debouce
        if (pin == 0)
        {
            option++;
        }

        switch (option)
        {
            case 1:
                vTaskPrioritySet(task1, 5);
                vTaskPrioritySet(task2, 4);
                break;
            case 2:
                vTaskPrioritySet(task1, 5);
                vTaskPrioritySet(task2, 5);
                break;
            case 3:
                vTaskPrioritySet(task1, 4);
                vTaskPrioritySet(task2, 5);
                break;
            default:
                option = 1;
                break;
        }
        vTaskDelayUntil(&firstTime, interval);
    }
}

void setupClock(void)
{
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);
}

void setupLed(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1 | GPIO_PIN_0);
}

void setupSW(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)){}
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


int main(void)
{
    setupClock();
    setupLed();
    setupSW();
    xTaskCreate((TaskFunction_t)Task1, "TurnOn", configMINIMAL_STACK_SIZE, NULL, 4, &task1 );
    xTaskCreate((TaskFunction_t)Task2, "TurnOff", configMINIMAL_STACK_SIZE, NULL, 5, &task2 );
    xTaskCreate((TaskFunction_t)Task3, "Polling", configMINIMAL_STACK_SIZE, NULL, 7, &task3 );   
    vTaskStartScheduler();
    while (1){}
}
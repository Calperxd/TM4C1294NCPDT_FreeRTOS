#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/*Setup Lib*/
#include "setup.h"
#include "floatpointUtils.h"
#include "microFFT.h"
#include "Potentiometer.h"


/* Scheduler includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/*Linear Regression Lib*/
#include "LinearRegression.h"

// TCB
static TaskHandle_t taskLED = NULL;
static TaskHandle_t taskADC = NULL;
static TaskHandle_t taskPOT = NULL;
static TaskHandle_t taskTimeout = NULL;

// Table
LinearInterpolationTable table;

// Buffer adc
uint32_t pui32ADC0Value[1];

char angular[5];
char linear[5];


// turn on
void TaskLED(void *pvParameters)
{
    while (1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void TaskADC(void *pvParameters)
{
    uint8_t snum[4];
    //char ampBuff[5];
    //char freqBuff[5];
    //ftoa(amp,ampBuff,4);
    //ftoa(freq,freqBuff,4);
    while (1)
    {
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false)){}
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
        itoa(pui32ADC0Value[0], snum, 4);
        UARTCharPut(UART0_BASE, snum[0]);
        UARTCharPut(UART0_BASE, snum[1]);
        UARTCharPut(UART0_BASE, snum[2]);
        UARTCharPut(UART0_BASE, snum[3]);
        UARTCharPut(UART0_BASE, 10);
        vTaskDelay(1);
    }
}

void TaskPOT(void *pvParameters)
{
    uint32_t buttom;
    uint32_t buttom2;
    // Pin CS must be high
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_2, (1<<GPIO_PIN_2));
    while (1)
    {
        buttom = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0);
        buttom2 = GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1);
        if(buttom == 0)
        {
            up();
            UARTCharPut(UART0_BASE, 64);
        }
        if (buttom2 == 0)
        {
            down();
            UARTCharPut(UART0_BASE, 65);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
}

void TaskTimeout(void *pvParameters)
{
    vTaskSuspend(taskLED);
    //vTaskSuspend(taskPOT);
    vTaskDelay(pdMS_TO_TICKS(15000));
    vTaskSuspend(taskADC);
    UARTCharPut(UART0_BASE, 10);
    vTaskResume(taskLED);
    //vTaskResume(taskPOT);
    while (1)
    {
       /* UARTCharPut(UART0_BASE, angular[0]);
        UARTCharPut(UART0_BASE, angular[1]);
        UARTCharPut(UART0_BASE, angular[2]);
        UARTCharPut(UART0_BASE, angular[3]);
        UARTCharPut(UART0_BASE, angular[4]);
        UARTCharPut(UART0_BASE, 10);
        UARTCharPut(UART0_BASE, 13);
        UARTCharPut(UART0_BASE, linear[0]);
        UARTCharPut(UART0_BASE, linear[1]);
        UARTCharPut(UART0_BASE, linear[2]);
        UARTCharPut(UART0_BASE, linear[3]);
        UARTCharPut(UART0_BASE, linear[4]);
        UARTCharPut(UART0_BASE, 10);
        UARTCharPut(UART0_BASE, 13);
        vTaskDelay(pdMS_TO_TICKS(1000));*/
    }
    
}

int main(void)
{
    setupClock();
    setupLed();
    setupSW();
    setupUart();
    setupADC();
    setupDigitalPotentiometer();
    static int x[4] = {1,2,3,4};
    static float y[4] = {701.4, 1201, 1713, 2197};
    table.maxIndex = 4;
    table.x = x;
    table.y = y;
    tableRegister(table);

    //char ampBuff[5];
    //char freqBuff[5];
    ftoa(getAngularCoef(),angular,5);
    ftoa(getLinearCoef(),linear,5);




    xTaskCreate((TaskFunction_t)TaskLED, "LED", configMINIMAL_STACK_SIZE, NULL, 4, &taskLED );
    xTaskCreate((TaskFunction_t)TaskADC, "ADC", configMINIMAL_STACK_SIZE, NULL, 4, &taskADC );
    xTaskCreate((TaskFunction_t)TaskPOT, "Potentiometer", configMINIMAL_STACK_SIZE, NULL, 4, &taskPOT );
    xTaskCreate((TaskFunction_t)TaskTimeout, "Timeout", configMINIMAL_STACK_SIZE, NULL, 4, &taskTimeout );
    vTaskStartScheduler();
    while (1){}
}
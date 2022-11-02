/*Setup Lib*/
#include "setup.h"
#include "floatpointUtils.h"
#include "microFFT.h"


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

/*Linear Regression Lib*/
#include "LinearRegression.h"

// TCB
static TaskHandle_t task1 = NULL;
static TaskHandle_t taskADC = NULL;

// Table
LinearInterpolationTable table;

// Buffer adc
uint32_t pui32ADC0Value[1];


/* FFT Parameters and Variables */
#define FFT_samples             1024      //This value MUST ALWAYS be a power of 2
#define FFT_samplingFrequency   200
#define FFT_ampFactor           8.35f
float vReal[FFT_samples];
float vImag[FFT_samples];

/* Simulated signal */
// Sinusoidal Parameters and Variables
#define sin_signals_number      5
float sin_amplitude[sin_signals_number] = {10,     20,    30,    40,    50};
float sin_frequency[sin_signals_number] = {10,     20,    30,    40,    50};
float amp, freq;


// Signal Parameters and Variable
#define signal_samples          FFT_samples
#define signal_Offset           20000
float signal[signal_samples];

/* Auxiliar Functions */
// Generate a signal from the sum of sinusoidal signals
void SignalGenerator(float* signal_output, size_t signal_output_len, size_t signals_number, float* signals_amplitud, float* signals_frequency, uint16_t signal_offset, uint16_t sampling_frequency);
// Remove the offset of a set of samples
void RemoveOffset(float* array, size_t array_len);


// turn on
void Task1(void *pvParameters)
{

    static int x[4] = {1,2,3,4};
    static float y[4] = {701.4,1201,1713,2197};

    table.maxIndex = 4;
    table.x = x;
    table.y = y;
    tableRegister(table);

    char res[20];
    float n = getAngularCoef();
    ftoa(n, res, 4);
    while (1)
    {
        /*UARTCharPut(UART0_BASE, res[0]);
        UARTCharPut(UART0_BASE, res[1]);
        UARTCharPut(UART0_BASE, res[2]);
        UARTCharPut(UART0_BASE, res[3]);
        UARTCharPut(UART0_BASE, res[4]);
        UARTCharPut(UART0_BASE, res[5]);*/
        // convert 123 to string [buf]
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}


void TaskADC(void *pvParameters)
{
    uint32_t amostras = 0;
    char snum[5];

    FFT_Init(vReal, vImag, FFT_samples, FFT_samplingFrequency);
    // Reading of the signal
    SignalGenerator(signal, signal_samples, sin_signals_number, sin_amplitude, sin_frequency, signal_Offset, FFT_samplingFrequency);
    // Signal Conditioning
    RemoveOffset(signal, signal_samples);
    // Memory copy
    memcpy(vReal, signal, sizeof(float)*signal_samples);
    FFT_Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Recommended use for high frequencies. Relative to Sampling Frequency. In this case: 70Hz
    FFT_Compute(FFT_FORWARD);
    FFT_ComplexToMagnitude();
    FFT_MajorPeak(&amp, &freq, FFT_ampFactor);
    char ampBuff[5];
    char freqBuff[5];
    ftoa(amp,ampBuff,4);
    ftoa(freq,freqBuff,4);
    UARTCharPut(UART0_BASE, ampBuff[0]);
    UARTCharPut(UART0_BASE, ampBuff[1]);
    UARTCharPut(UART0_BASE, ampBuff[2]);
    UARTCharPut(UART0_BASE, ampBuff[3]);
    UARTCharPut(UART0_BASE, ampBuff[4]);
    UARTCharPut(UART0_BASE, 13);    ///r carriage return
    UARTCharPut(UART0_BASE, 10);    // /n fim de linha

    UARTCharPut(UART0_BASE, freqBuff[0]);
    UARTCharPut(UART0_BASE, freqBuff[1]);
    UARTCharPut(UART0_BASE, freqBuff[2]);
    UARTCharPut(UART0_BASE, freqBuff[3]);
    UARTCharPut(UART0_BASE, freqBuff[4]);
    UARTCharPut(UART0_BASE, 13);    ///r carriage return
    UARTCharPut(UART0_BASE, 10);    // /n fim de linha*/


    while (1)
    {
        /*ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
        itoa(pui32ADC0Value[0], snum, 10);
        //UARTCharPut(UART0_BASE, snum[0]);
        //UARTCharPut(UART0_BASE, snum[1]);
        //UARTCharPut(UART0_BASE, snum[2]);
        //UARTCharPut(UART0_BASE, snum[3]);
        //UARTCharPut(UART0_BASE, snum[4]);
        //UARTCharPut(UART0_BASE, 13);    ///r carriage return
        //UARTCharPut(UART0_BASE, 10);    // /n fim de linha
        amostras++;
        if (amostras == 150000)
        {
            //UARTCharPut(UART0_BASE, 0);
            //UARTCharPut(UART0_BASE, 13);    ///r carriage return
            //UARTCharPut(UART0_BASE, 10);    // /n fim de linha
            amostras = 0;
        }*/
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}




int main(void)
{
    setupClock();
    setupLed();
    setupSW();
    setupUart();
    setupADC();
    xTaskCreate((TaskFunction_t)Task1, "Send", configMINIMAL_STACK_SIZE, NULL, 4, &task1 );
    xTaskCreate((TaskFunction_t)TaskADC, "ADC", configMINIMAL_STACK_SIZE, NULL, 4, &taskADC );
    vTaskStartScheduler();
    while (1){}
}

void SignalGenerator(float* signal_output, size_t signal_output_len, size_t signals_number, float* signals_amplitud, float* signals_frequency, uint16_t signal_offset, uint16_t sampling_frequency)
{
    for (uint16_t i = 0; i < signals_number; i++)
    {
        float cycles = (((signal_output_len-1) * (float)signals_frequency[i]) / sampling_frequency); //Number of signal cycles that the sampling will read
        for (uint16_t j = 0; j < signal_output_len; j++)
        {
            signal_output[j] += (float)((signals_amplitud[i] * (sin((j * (6.28318531 * cycles)) / signal_output_len))) / 2.0);/* Build data with positive and negative values*/
        }
    }

    for (size_t i = 0; i < signal_output_len; i++)
    {
        signal_output[i] += signal_offset;
    }
}

void RemoveOffset(float* array, size_t array_len)
{
	// calculate the mean of vData
	float mean = 0;
	for (size_t i = 0; i < array_len; i++)
	{
		mean += array[i];
	}
	mean /= array_len;
	// Subtract the mean from vData
	for (size_t i = 0; i < array_len; i++)
	{
		array[i] -= mean;
	}
}

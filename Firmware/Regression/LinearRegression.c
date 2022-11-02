#include "LinearRegression.h"

static float angularCoef;
static float linearCoef;

void tableRegister(LinearInterpolationTable table)
{
    float xy;
    float x2;
    float x;
    float y;
    float x_avg;
    float y_avg;
    float a1;
    float a0;

    for (int i = 0; i < table.maxIndex; i++)
    {
        xy +=(table.x[i] * table.y[i]); 
    }
    // calculate de x2
    for (int i = 0; i < table.maxIndex; i++)
    {
        x2 += pow(table.x[i],2); 
    }
    // sum of x
    for (int i = 0; i < table.maxIndex; i++)
    {
        x += table.x[i];
    }
    // sum of y
    for (int i = 0; i < table.maxIndex; i++)
    {
        y += table.y[i];
    }
    // x avarage
    x_avg = x/table.maxIndex;
    // y avarage
    y_avg = y/table.maxIndex;
    a1 = ((table.maxIndex*xy) - x*y)/((table.maxIndex*x2)-pow(x,2));
    a0 = (y_avg - (a1*x_avg));
    angularCoef = a1;
    linearCoef = a0;
}

float getAngularCoef(void)
{
    return angularCoef;
}

float getLinearCoef(void)
{
    return linearCoef;
}
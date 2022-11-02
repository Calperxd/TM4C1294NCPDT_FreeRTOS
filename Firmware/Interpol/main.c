#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct LinearInterpolationTable
{
    int maxIndex;
    int *x;
    float *fx;
}LinearInterpolationTable;


void tableRegister(LinearInterpolationTable table)
{
    float *auxFx;
    int *auxX;
    float angularCoef;
    float linearCoef;
    for (int i = 0; i < table.maxIndex; i++)
    {
        if ((i+1) > (table.maxIndex - 1))
        {
            break;
        }
        angularCoef = (table.fx[i+1] - table.fx[i])/(table.x[i+1] - table.x[i]);
        linearCoef = (-(angularCoef)*(table.x[i])) + (table.fx[i]);
    }
    printf("/n The angular coef %.2f /n",angularCoef);
}

float getAngularCoef(void)
{

}

float getLinearCoef(void)
{

}


int main(void)
{
    static int x[4] = {1,2,3,4};
    static float fx[4] = {701.4,1201,1713,2197};
    LinearInterpolationTable table;
    table.maxIndex = 4;
    table.x = x;
    table.fx = fx;
    tableRegister(table);
    return 0;
}
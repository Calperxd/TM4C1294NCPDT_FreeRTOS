#include <stdio.h>
#include <stdlib.h>
#include "LinearRegression.h"

LinearInterpolationTable table;

int main(void)
{
    static int x[4] = {1,2,3,4};
    static float y[4] = {701.4,1201,1713,2197};

    table.maxIndex = 4;
    table.x = x;
    table.y = y;
    tableRegister(table);

    printf("\n the fuction is fx = %.2fx %.2f\n",getAngularCoef(),getLinearCoef());
    return 0;
}
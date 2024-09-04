#include <stdio.h>

typedef struct 
{
    float min;
    float max;
} clamp_param;


float clamp(clamp_param *param, float value) {
    if (value < param->min)
        return param->min;
    else if (value > param->max)
        return param->max;
    else
        return value;
}

int main(void) 
{
    clamp_param motor_driver;
    motor_driver.min = 100.0f;
    motor_driver.max = 300.0f;

    float temp = clamp(&motor_driver, 150.0f);
    printf("%f\n", temp);
    temp = clamp(&motor_driver, 50.0f);
    printf("%f\n", temp);
    temp = clamp(&motor_driver, 350.0f);
    printf("%f\n", temp);
}
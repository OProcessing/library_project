#include <stdio.h>
#include <math.h>

typedef struct
{
	float reference;
    float current_value;
	float prev_error;
	float error;
	float Kp, Ki, Kd;
	float P, I, D;
	float value;

} PIDparameter;


void PID_Init(PIDparameter *PID, float Kp, float Ki, float Kd)
{
    PID->reference = 0.0f;
    PID->current_value = 0.0f;
    PID->prev_error = 0.0f;
    PID->error = 0.0f;
    PID->Kp = Kp;
    PID->Ki = Ki;
    PID->Kd = Kd;
    PID->P = 0.0f;
    PID->I = 0.0f;
    PID->D = 0.0f;
    PID->value = 0.0f;
}

float PID_Compute(PIDparameter *PID)
{
    PID->error = PID->reference - PID->current_value;
    
    PID->P = PID->Kp * PID->error;
    PID->I += PID->Ki * PID->error;
    PID->D = PID->Kd * (PID->error - PID->prev_error);
    
    PID->value = PID->P + PID->I + PID->D;

    PID->prev_error = PID->error;
    
    return PID->value;
}

int main(void)
{
    PIDparameter PIDcontroller;

    PID_Init(&PIDcontroller, 1.0f, 0.1f, 0.05f);
    PIDcontroller.reference = 10.0f;
    PIDcontroller.current_value = 0.0f;

    PID_Compute(&PIDcontroller);
}
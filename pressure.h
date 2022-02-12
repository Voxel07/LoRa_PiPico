#ifndef _PRESSURE_H_
#define _PRESSURE_H_

typedef struct PressureSensor
{
    long SensoreId;
    int pressure;
} PressureSensor_t;

void pressure_addSensorId(PressureSensor_t *pressure, long id);
void pressure_addPressureValue(PressureSensor_t *pressure, int value);

#endif
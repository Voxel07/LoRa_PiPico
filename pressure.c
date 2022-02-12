#include "pressure.h"

void pressure_addSensorId(PressureSensor_t *pressure, long id)
{
    pressure->SensoreId = id;
}
void pressure_addPressureValue(PressureSensor_t *pressure, int value)
{
    pressure->pressure = value;
}
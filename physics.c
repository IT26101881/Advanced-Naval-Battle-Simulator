#include <math.h>
#include "physics.h"

#define PI 3.141592653589793

double calculateRange(double velocity, double angle)
{
    double gravity = 9.81;
    double angleRadians;

    angleRadians = angle * PI / 180.0;

    return (velocity * velocity *
            sin(2 * angleRadians)) / gravity;
}

double calculateFlightTime(double velocity, double angle)
{
    double gravity = 9.81;
    double angleRadians;

    angleRadians = angle * PI / 180.0;

    return (2 * velocity * sin(angleRadians)) / gravity;
}

double calculateFiringAngle(double velocity, double distance)
{
    double gravity = 9.81;
    double value;
    double angleRadians;

    if (velocity <= 0)
    {
        return -1;
    }

    value = (distance * gravity) /
            (velocity * velocity);

    if (value > 1 || value < 0)
    {
        return -1;
    }

    angleRadians = asin(value) / 2.0;

    return angleRadians * 180.0 / PI;
}

double calculateDistance(double x1, double y1,
                         double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) +
                (y2 - y1) * (y2 - y1));
}



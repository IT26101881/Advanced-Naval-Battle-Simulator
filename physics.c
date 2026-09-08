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

double calculateFiringAngle(double velocity,
                            double distance,
                            double minimumAngle)
{
    double gravity = 9.81;
    double value;
    double lowAngle;
    double highAngle;

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

    /*
     * Calculate the two possible firing angles.
     */
    lowAngle = asin(value) / 2.0;
    lowAngle = lowAngle * 180.0 / PI;

    highAngle = 90.0 - lowAngle;

    /*
     * Use the low angle if it satisfies
     * the minimum firing angle.
     */
    if (lowAngle >= minimumAngle)
    {
        return lowAngle;
    }

    /*
     * Otherwise, try the high-angle solution.
     */
    if (highAngle >= minimumAngle &&
        highAngle <= 90.0)
    {
        return highAngle;
    }

    return -1;
}

double calculateDistance(double x1, double y1,
                         double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) +
                (y2 - y1) * (y2 - y1));
}

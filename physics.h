#ifndef PHYSICS_H
#define PHYSICS_H

double calculateRange(double velocity, double angle);

double calculateFlightTime(double velocity, double angle);

double calculateFiringAngle(double velocity, double distance, double minimumAngle);

double calculateDistance(double x1, double y1,
                         double x2, double y2);

#endif

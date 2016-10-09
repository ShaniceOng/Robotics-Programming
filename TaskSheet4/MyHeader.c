#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MyHeader.h"

//GET ANGLE USING LEFT AND RIGHT DISTANCES
float getAngle(float distLeft, float distRight)
{
    return ((distLeft - distRight)/105.8);
}

//GET RL AND RR VALUES FOR CALCULATING RM
float getR(float currAngle, float dist)
{
  return (dist/currAngle);
}

//GET THE DISTANCE IN Y DIRECTION (dY)
float getYDistance(float currAngle, float rm)
{
    return (rm * sin(currAngle));
}

//GET THE DITANCE IN X DIRECTION (dX)
float getXDistance(float currAngle, float rm)
{
    return (rm - (rm*cos(currAngle)));
}

//GET DISTANCE TRAVELLED USING PYTHAGORAS THEOREM
float getDistanceMoved(float dX, float dY)
{
    return sqrt((dX*dX) + (dY*dY));
}
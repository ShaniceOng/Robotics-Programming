#include "simpletools.h" 
#include "abdrive.h"
#include <math.h>
#include <stdlib.h>
#include "MyHeader.h"
#include "ping.h"

int distLeft;
int currLeft;
int distRight;
int currRight; 
float currAngle;
float tickDistance = 3.25;
float dY, dX;
float totalDistance;

//CALCULATE THE PROPORTIONAL VALUE FOR PID
int calculateProportional(int target, int measured)
{
  return (measured - target);
}
 
//CALCULATE THE DERIVATIVE FOR PID
float calculateDerivative(float constant, float difference)
{
  return (difference/constant);
}

//DRIVE AND TURN ACCORDING TO DISTANCE BETWEEN ROBOT AND LEFT WALL
void turn(int difference)
{
  drive_speed(40-(1.7*difference), 40+(1.7*difference));
}    

//CHECK FOR A WALL IN FRONT OF THE ROBOT
int obstacleCheck()
{
  int distance = ping_cm(8);
 
  if (distance<10)
  {
    return 0;
  }
 
  else
  {
    return 1;
  }
  pause(5);
}      

//GET DISTANCE FROM LEFT SENSOR 
int getDistance()
{
  int irLeft = 0;
 
  for(int dacVal = 0; dacVal < 160; dacVal += 8)  
    {                                               
      dac_ctr(26, 0, dacVal);                       
      freqout(11, 1, 38000);                
 
      irLeft += input(10);                          
    }  
  return irLeft;
}      

//MAIN FUNCTION TO CONTROL ROBOT MOVEMENT, CARRY OUT CALCULATIONS AND SAVE INFORMATION ON THE SD CARD
int main()
{ 
  float integral = 0;
  float derivative = 0;
  float target = 8;
  float prevError = 0;

  //WHILE PING SENSOR DISTANCE < 10, KEEP MOVING
  while(obstacleCheck()){
   
    int currentPosition = getDistance(); //get distance between robot and left wall
    float currError = calculateProportional(target, currentPosition); //calculate proportional for PID
    integral += integral*0.35; //calculate integral for PID
    derivative = (currError-prevError); //calculate derivative for PID
    float calculatedError = (0.6*currError) + integral + (0.7*derivative); //calculate total error
    turn((int)(3.0*calculatedError)); //change robot's speed
    prevError = currError; //set current error as previous error for the next iteration
   
  pause(80);
 
}  
  drive_speed(0,0); //robot stops when it is < 10cm away from a wall in front of it
  
  //CALCULATE DISTANCE AND ANGLE TRAVELLED
  drive_getTicks(&currLeft, &currRight);
  distLeft = currLeft*tickDistance;
  distRight = currRight*tickDistance;  
  currAngle = getAngle(distLeft, distRight);
  float rm = (getR(currAngle, distLeft)+getR(currAngle, distRight))/2;
  totalDistance = getDistanceMoved(getXDistance(currAngle, rm), getYDistance(currAngle, rm));
  
  //PRINT STATEMENTS USED TO PRINT DISTANCE AND ANGLE TO TERMINAL
  //print("d=%f\n", totalDistance);
  //print("a=%f\n", currAngle);
  
  //WRITING VALUES TO THE SD CARD
  sd_mount(22, 23, 24, 25);                 

  FILE* fp = fopen("t.txt", "w"); 
  
  fwrite(&totalDistance, sizeof(float), 1, fp);      
  fwrite("\n", 1, 1, fp);
  fwrite(&currAngle, sizeof(float), 1, fp);
  fwrite("\n", 1, 1, fp);
  fclose(fp);                

} 

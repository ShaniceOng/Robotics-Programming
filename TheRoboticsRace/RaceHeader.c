#include "simpletools.h" 
#include "abdrive.h"
#include <stdlib.h>
#include "RaceHeader.h"
#include "ping.h"

void turnRight()
{
    drive_goto(26,-25);
}

void turnLeft()
{
    drive_goto(-25,26);
}

void adjust()
{
    int irLeft = 0;                                     
    int irRight = 0;                                    
    int sideDist;
    for(int dacVal = 0; dacVal < 160; dacVal += 8)  
    {                                               
        dac_ctr(26, 0, dacVal);                       
        freqout(11, 1, 38000);                        
        irLeft += input(10);                          
        
        dac_ctr(27, 1, dacVal);                       
        freqout(1, 1, 38000);
        irRight += input(2);                          
    }
    printf("irLeft = %i\n", irLeft);
    printf("irRight = %i\n", irRight);
    if ((irRight<20)&&(irLeft<20))
    {
        if ((irLeft-irRight)<-2)
        {
            sideDist = irRight - irLeft;
            drive_goto((sideDist*0.6),0);
        }
        else if ((irLeft-irRight)>2)
        {
            sideDist = irLeft - irRight;
            drive_goto(0,(sideDist*0.6));
        }
    }
    else if (irRight<20)
    {
        sideDist = 9-irRight;
        drive_goto(-(sideDist*0.5),0);
    }
    else if (irLeft<20)
    {
        sideDist = 9-irLeft;
        drive_goto(0,-(sideDist*0.5));
    }
    
}

int getNextIndex (int index, int currentNodeCount)
{
    if(index==0)
    {
        return currentNodeCount+4;
    }
    else if(index==1)
    {
        return currentNodeCount+1;
    }
    else if(index==2)
    {
        return currentNodeCount-4;
    }
    else
    {
        return currentNodeCount-1;
    }
}

int getBackIndex (int index, int currentNodeCount)
{
    if(index==0)
    {
        return currentNodeCount-4;
    }
    else if(index==1)
    {
        return currentNodeCount-1;
    }
    else if(index==2)
    {
        return currentNodeCount+4;
    }
    else
    {
        return currentNodeCount+1;
    }
}

int obstacleCheck()
{
    int distance = ping_cm(8);
    
    return distance;
}

void adjustFront ()
{
    int dist = obstacleCheck();
    if (dist<16)
    {
        int moveDist = ((16-dist)*10)/3.25;
        drive_goto(-moveDist, -moveDist);
    }
    else if (dist>16 && dist<32)
    {
        int moveDist = ((dist-16)*10)/3.25;
        drive_goto(moveDist, moveDist);
    }
}

void blink()
{
    high(26);
    pause(100);
    low(26);
    high(26);
    pause(100);
    low(26);
    high(26);
    pause(100);
    low(26);
}

int calculateProportional(int target, int measured)
{
    int difference = measured - target;
    return difference;
}

void turn(int difference)
{
    drive_speed(60-(1.8*difference), 60+(1.8*difference));
}

void driveForward (int distance)
{
    float integral = 0;
    float derivative = 0;
    float target = 20;
    float prevError = 0;
    int speedLeft = 100;
    int speedRight = 100;
    
    int left, right;
    drive_getTicks(&left, &right);
    int newLeft = left;
    int newRight = right;
    
    //WHILE PING SENSOR DISTANCE < 10, KEEP MOVING
    while((newRight-right)<distance)
    {
        drive_speed(speedLeft, speedRight);
        drive_getTicks(&newLeft, &newRight);
        int irLeft = 0;                                     
        int irRight = 0;                                    
        for(int dacVal = 0; dacVal < 160; dacVal += 8)  
        {                                               
            dac_ctr(26, 0, dacVal);                       
            freqout(11, 1, 38000);                        
            irLeft += input(10);                          
            
            dac_ctr(27, 1, dacVal);                       
            freqout(1, 1, 38000);
            irRight += input(2);                          
        }
    
        if (irRight<20 && irLeft<20 && ((irLeft-irRight)>=2 || (irLeft-irRight)<=-2))
        {
            float currError = calculateProportional(irRight, irLeft); //calculate proportional for PID
            derivative = (currError-prevError); //calculate derivative for PID
            turn((int)(2.0*derivative)); //change robot speed
            prevError = currError; //set current error as previous error for next iteration
        }
        else if (irRight<18 && irLeft==20)
        {
            float currError = calculateProportional(irRight, 9); //calculate proportional for PID
            derivative = (currError-prevError); //calculate derivative for PID
            turn((int)(2.0*derivative)); //change robot speed
            prevError = currError; //set current error as previous error for next iteration
        }
        else if (irRight==20 && irLeft<18)
        {
            float currError = calculateProportional(9, irLeft); //calculate proportional for PID
            derivative = (currError-prevError); //calculate derivative for PID
            turn((int)(2.0*derivative)); //change robot speed
            prevError = currError; //set current error as previous error for next iteration
        }
         
        else
        {
            drive_speed(60,60);
        }
        
        pause(80);
        
        if (obstacleCheck() <=14)
        {
            break;
        }
        
    }
    drive_speed(0,0);
}



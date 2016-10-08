/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"  
#include "abdrive.h"                    // Include simple tools
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

int arraysIndex;
int left[20];
int leftPosition[20];

//Bubble Sort function (input array, array size)
void bubbleSort(int *array, long unsigned n)
{

    int currentNum;
    bool switched = true;
    while (switched==true)
    {
        switched=false;
        for (int i=1; i<n; i++)
        {
            
            if (array[i]<array[i-1])
            {
              //arrays to save positions at which a swap takes place
                left[arraysIndex] = array[i-1];
                leftPosition[arraysIndex] = i-1;
                arraysIndex++;
                
                //swap numbers to sort them
                currentNum = array[i];
                array[i]=array[i-1];
                array[i-1]=currentNum;
                switched=true;
            }
        }
        n-=1;
    }
}

int main() {
    int arraySize = 5;
    
    int toSort[5] = {12,2,75,29,1};
   
    bubbleSort(toSort, arraySize);
     
    
    int y = 49; //half the distance between the center lines of two adjacent numbers
    int difference; //to store difference between next and current positions
    int distance; //to store the distance that the robot has to travel
    
    int prevPos = 0; //starting position of the robot
    drive_goto(y,y); //drive to the starting position
    
    
    for (int k=0; k<arraysIndex; k++)
    {
      if (leftPosition[k]<prevPos)
      {
        drive_goto(-26,25);
        drive_goto(-26,25);
        difference = prevPos - leftPosition[k];
        distance = difference*2*y;
        drive_goto(distance, distance);
        drive_goto(25,-26);
        prevPos = leftPosition[k];
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        drive_goto(25,-26);
        
      }
      else if (leftPosition[k]>prevPos) 
      {       
        difference = leftPosition[k] - prevPos;
        distance = difference*2*y;
        drive_goto(distance, distance);
        drive_goto(-26,25);
        prevPos = leftPosition[k];
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        drive_goto(25,-26);
      }
      else
      {
        drive_goto(-26,25);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        drive_goto(25,-26);
      }
      //after last swap, return to the middle of the array
      if (k==(arraysIndex-1))
      {
        drive_goto(-26,25);
        drive_goto(-26,25);
        difference = abs(leftPosition[k] - 2);
        distance = difference*2*y;
        if (leftPosition[k]>2)
        {
          drive_goto(distance+y, distance+y);
        }
        else if (leftPosition[k]<2)
        {
          drive_goto(-distance+y, -distance+y); 
        }
        drive_goto(25,-26);
      }                           
    }                      
}
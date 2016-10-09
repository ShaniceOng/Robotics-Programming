/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int toPegs[30];
int fromPegs[30];
int indexPeg;

void addCounter (char pegLetter, int counter[3])
{
    if (pegLetter=='A')
    {
        counter[0]++;
        toPegs[indexPeg] = 0;
        
    }
    else if (pegLetter=='B')
    {
        counter[1]++;
        toPegs [indexPeg] = 1;
    }
    else if (pegLetter=='C')
    {
        counter[2]++;
        toPegs [indexPeg] = 2;
    }
}

void subtractCounter (char pegLetter, int counter[3])
{
    if (pegLetter=='A')
    {
        counter[0]--;
        fromPegs [indexPeg] = 0;
    }
    else if (pegLetter=='B')
    {
        counter[1]--;
        fromPegs [indexPeg] = 1;
    }
    else if (pegLetter=='C')
    {
        counter[2]--;
        fromPegs [indexPeg] = 2;
    }
}

void tower(int discNum, char fromPeg, char toPeg, char otherPeg, int counter[3])
{
    if (discNum==1)
    {
        addCounter(toPeg, counter);
        subtractCounter(fromPeg, counter);
        indexPeg++;
        
        return;
    }
    
    tower(discNum-1, fromPeg, otherPeg, toPeg, counter);
    addCounter(toPeg, counter);
    subtractCounter(fromPeg, counter);
    indexPeg++;
    
    tower(discNum-1, otherPeg, toPeg, fromPeg, counter);
}

void towerHanoi (int discNum, char firstPeg, int counter[3])
{
    
    if (firstPeg == 'A')
    {
        char fromLetter = 'A';
        char toLetter = 'B';
        char otherLetter = 'C';
        counter[0] = discNum;
        counter[1] = 0;
        counter[2] = 0;
        tower(discNum, fromLetter,toLetter, otherLetter, counter);
        
    }
    
    else if (firstPeg=='B')
    {
        char fromLetter = 'B';
        char toLetter = 'C';
        char otherLetter = 'A';
        counter[0] = 0;
        counter[1] = discNum;
        counter[2] = 0;
        tower(discNum, fromLetter,toLetter, otherLetter, counter);
        
    }
    else if (firstPeg == 'C')
    {
        char fromLetter = 'C';
        char toLetter = 'A';
        char otherLetter = 'B';
        counter[0] = 0;
        counter[1] = 0;
        counter[2] = discNum;
        tower(discNum, fromLetter,toLetter, otherLetter, counter);
        
    }
}

int main(void)
{
    int discsNum = 4;
    char startPeg = 'B';
    
    int counter[3];
    
    towerHanoi(discsNum, startPeg, counter);
    
    int y = 98;
    
    int currentPos = 0;
    int toPos = fromPegs[0];
    
    int distance = (toPos - currentPos)*y;
    
    drive_goto(distance, distance);
    high(26);
    pause(1000);
    low(26);
    pause(1000);
    high(26);
    pause(1000);
    low(26);
    pause(1000);
    
    for (int i=0; i<indexPeg; i++)
    {
      currentPos = fromPegs[i];
      toPos = toPegs[i];
      
      int difference = abs(toPos-currentPos);
      
      distance = difference*y;
      
      if (toPos>currentPos)
      {
        drive_goto(distance, distance);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      }
      
      else if (toPos<currentPos)
      {
        drive_goto(-distance, -distance);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      } 
      else if (toPos==currentPos)
      {
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      }
      
      int nextPos = fromPegs[i+1];
      
      difference = abs(nextPos-toPos);
      
      distance = difference*y;
      
      if (nextPos>toPos)
      {
        drive_goto(distance, distance);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      }
      
      else if (nextPos<toPos)
      {
        drive_goto(-distance, -distance);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      } 
      else if (toPos==nextPos)
      {
        high(26);
        pause(1000);
        low(26);
        pause(1000);
        high(26);
        pause(1000);
        low(26);
        pause(1000);
      }
                             
    }      
              
    
}
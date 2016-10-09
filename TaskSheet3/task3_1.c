/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include <stdio.h>
#include <stdlib.h>

void addCounter (char pegLetter, int counter[3])
{
    if (pegLetter=='A')
    {
        counter[0]++;
    }
    else if (pegLetter=='B')
    {
        counter[1]++;
    }
    else if (pegLetter=='C')
    {
        counter[2]++;
    }
}

void subtractCounter (char pegLetter, int counter[3])
{
    if (pegLetter=='A')
    {
        counter[0]--;
    }
    else if (pegLetter=='B')
    {
        counter[1]--;
    }
    else if (pegLetter=='C')
    {
        counter[2]--;
    }
}

void printCounter (int counter[3])
{
    printf("A: %i | B: %i | C: %i\n", counter[0], counter[1], counter[2]);
}

void tower(int discNum, char fromPeg, char toPeg, char otherPeg, int counter[3])
{
    if (discNum==1)
    {
        printf("Move disc from %c to %c\n", fromPeg, toPeg);
        addCounter(toPeg, counter);
        subtractCounter(fromPeg, counter);
        printCounter(counter);
        
        return;
    }
    tower(discNum-1, fromPeg, otherPeg, toPeg, counter);
    printf("Move disc from %c to %c\n", fromPeg, toPeg);
    addCounter(toPeg, counter);
    subtractCounter(fromPeg, counter);
    printCounter(counter);
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
        char toLetter = 'A';
        char otherLetter = 'C';
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
    int discsNum;
    char startPeg;
    printf("Enter the number of discs: \n");
    scanf("%i", &discsNum);
    printf("Enter the starting peg's letter: \n");
    scanf(" %c", &startPeg);
    
    int counter[3];
    
    towerHanoi(discsNum, startPeg, counter);
    
}
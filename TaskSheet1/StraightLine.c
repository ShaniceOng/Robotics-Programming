/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"  
#include "abdrive.h"                    // Include simple tools

int main()                                    // Main function
{
  drive_goto(308,308);
  pause(8000);
  drive_goto(0,0);
   
}

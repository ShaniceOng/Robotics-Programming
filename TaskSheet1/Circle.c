/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"  
#include "abdrive.h"                    // Include simple tools

int main()                                    // Main function
{
  while(1) {
    drive_goto(102,-102);
  }    
  
}

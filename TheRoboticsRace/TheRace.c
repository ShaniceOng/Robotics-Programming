/*
 Blank Simple Project.c
 http://learn.parallax.com/propeller-c-tutorials
 */
#include "simpletools.h"
#include "abdrive.h"
#include <math.h>
#include <stdlib.h>
#include "ping.h"
#include <stdio.h>
#include "RaceHeader.h"

int distLeft;
int currLeft;
int distRight;
int currRight;
float currAngle;
float tickDistance = 3.25;
int driveTicks = 118;
int driveTicksTwo = 118;
int checkDist = 18;
int turnCount = 0;
int currentNodeCount = 0;
int sideDistance = 2;
int sideDistance2 = 2;
int driveBackTicks = 129;

//struct for each square in the maze
struct node
{
    int neighbors[4]; //to keep either the index of the neighbor or -1 (if there is no path to the neighbor)
    //neighbors[0] = North, neighbors[1] = East, neighbors[2] = South, neighbors[3] = West
    int visited; //either 0 (not visited) or 1 (visited) for mapping out the maze
    int bfsVisited; //either 0 (not visited) or 1 (visited) for finding the shortest path
};

struct node nodes[16]; //array to hold a node for each square in the 16-square maze

void checkSides(struct node currNode, int currentNodeCount) //function maps out the maze by recursive DFS on each node
{
    if(nodes[currentNodeCount].visited==0) //only check the neighbors if the node has not been visited yet
    {
        for(int n=0; n<4; n++) //check each of the four sides
        {
            int index = turnCount%4;
            
            int neighborIndex = getNextIndex(index,currentNodeCount);
            
            int dist = obstacleCheck();
            if(dist>29) //if there is no wall in front of the robot
            {
                if ((neighborIndex>=0 && neighborIndex<16)) //if the node exists in the maze
                {
                    nodes[currentNodeCount].neighbors[index] = neighborIndex; //save the index of the neighbor in the current node's neighbors array
                }
            }
            adjust();
            if (dist<14) //adjust robot position
            {
                int moveDist = ((14-dist)*10)/3.25;
                drive_goto(-moveDist, -moveDist);
            }
            else if (dist>14 && dist<32)
            {
                int moveDist = ((dist-14)*10)/3.25;
                drive_goto(moveDist, moveDist);
            }
            
            turnRight(); //turn to check the next side
            turnCount++;
        }
        nodes[currentNodeCount].visited = 1; //mark the current node as visited
    }
    nodes[currentNodeCount].visited = 1;
    
    for(int d=0; d<4; d++) //recursively visit each of the neighbors that this node has a path to that has not already been visited
    {
        int index = turnCount%4;
        if((nodes[currentNodeCount].neighbors[index]!=-1) && (nodes[nodes[currentNodeCount].neighbors[index]].visited==0))
        {
            adjust();
            driveForward(driveTicks);
            checkSides(nodes[nodes[currentNodeCount].neighbors[index]],nodes[currentNodeCount].neighbors[index]);
            
            turnRight();
            turnCount++;
        }
        else
        {
            turnRight();
            turnCount++;
        }
        
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
    adjust();
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
    
    //after the current node's existing, connected neighbors have all been visited, turn 180 degrees and go back to previous node
    if (irLeft<irRight)
    {
      drive_goto(-52,52);
      driveForward(driveTicks);
    }
    else
    {
      drive_goto(52,-52);
      driveForward(driveTicks);
    }
    
    adjust();
    
    irLeft = 0;                                     
    irRight = 0;                                    
    for(int dacVal = 0; dacVal < 160; dacVal += 8)  
    {                                               
        dac_ctr(26, 0, dacVal);                       
        freqout(11, 1, 38000);                        
        irLeft += input(10);                          
        
        dac_ctr(27, 1, dacVal);                       
        freqout(1, 1, 38000);
        irRight += input(2);                          
    }
    
    if (irLeft<irRight)
    {
      drive_goto(-52,52);
    }
    else
    {
      drive_goto(52,-52);
    }
                
    int dist = obstacleCheck();
    if (dist<16)
    {
        int moveDist = ((16-dist)*10)/3.25;
        drive_goto(-moveDist, -moveDist);
    }
    else if (dist>16 && dist<32)
    {
        int moveDist = ((dist-16)*10)/3.25;
        driveForward(moveDist);
    }
    
    int index = turnCount%4;
    
}

int outerArray[16]; //to keep the shortest path found by the newDFS function
int minLength = 16; //length of shortest path (set to 16 so there is always a solution)

void newDFS (struct node currNode, int currArray[], int pathLength) //to find the shortest path to the opposite corner
{
    if (pathLength<=minLength) //only continue depth-first search if the path being explored is shorter than the current minimum
    {
        for (int j=0; j<4; j++)
        {
            if (currNode.neighbors[j]>-1)
            {
                int neighborIn = currNode.neighbors[j];
                
                if (nodes[neighborIn].bfsVisited==0)
                {
                    if (neighborIn == 15) //if the path reaches node 15, save the path as the shortest path
                    {
                        if ((pathLength+1)<minLength)
                        {
                            minLength = pathLength;
                            for (int w=0; w<pathLength; w++)
                            {
                                outerArray[w] = currArray[w];
                            }
                            outerArray[pathLength] = 15;
                        }
                    }
                    else
                    {
                        int neighborCount;
                        for (int w=0; w<4; w++)
                        {
                            if (nodes[neighborIn].neighbors[w]>-1)
                            {
                                neighborCount++;
                            }
                        }
                        if (neighborCount>1) //if the node is connected to any node other than the previous node
                        {
                            int newArray[pathLength+1];
                            for (int n=0; n<(pathLength);n++)
                            {
                                newArray[n] = currArray[n];
                            }
                            newArray[pathLength] = neighborIn;
                            printf("");
                            nodes[neighborIn].bfsVisited = 1; //mark the neighbor node as visited for this function
                            newDFS(nodes[neighborIn], newArray, pathLength+1); //continue DFS with neighbor node
                            nodes[neighborIn].bfsVisited = 0; //mark the neighbor node as not visited for this function
                            //the neighbor node is marked as unvisited again so that it can be used in finding other paths
                        }
                    }
                }
            }
        }
    }
}


//MAIN FUNCTION
int main()
{
    for(int i=0; i<16; i++)
    {
        for(int j=0; j<4;j++)
        {
            nodes[i].neighbors[j] = -1; //set all the nodes' neighbors to -1
        }
        nodes[i].visited = 0; //set all to unvisited
        nodes[i].bfsVisited = 0; //set all to unvisited for the shortest path function
        
    }
    
    drive_goto(130,130); //drive robot into the maze
    
    checkSides(nodes[currentNodeCount],currentNodeCount); // start recursively visiting from node 0
    
    drive_speed(0,0); //stop after all squares have been visited
    
    blink();
    
    nodes[0].bfsVisited = 1; //mark the starting node as visited
    
    int anArray[1] = {0}; //input array is {0} because every path starts with node 0
    
    newDFS(nodes[0],anArray,1); //call newDFS to update outerArray to hold the array representing the shortest path
    
    for (int i=0; i<3; i++) //blink three times
    {
        blink();
        pause(2000);
    }
    
    driveForward(driveTicks); //drive robot into the maze
    
    int i=0;
    int counter = 0;
    int totalDist;
    driveForward(131);
    while (i<minLength) //go through all node indices in the array of shortest path
    {
        totalDist = driveTicksTwo;
        
        int diff = outerArray[i+1] - outerArray[i];
        if (diff==4) //next square is above current square
        {
            int newDiff = outerArray[i+2] - outerArray[i+1];
            while (newDiff==4 && i<6)
            {
                totalDist+= driveTicksTwo;
                i++;
                newDiff = outerArray[i+2] - outerArray[i+1];
            }
            driveForward(totalDist);
            adjust();
        }
        else if (diff==1) //next square is to the right of the current square
        {
            turnRight();
            int newDiff = outerArray[i+2] - outerArray[i+1];
            while (newDiff==1 && i<6)
            {
                totalDist += driveTicksTwo;
                i++;
                newDiff = outerArray[i+2] - outerArray[i+1];
            }
            driveForward(totalDist);
            adjust();
            if (i!=5)
            {
                turnLeft();
            }
        }
        else if (diff==-1) //next square is to the left of the current square
        {
            turnLeft();
            int newDiff = outerArray[i+2] - outerArray[i+1];
            while (newDiff==4 && i<6)
            {
                totalDist+= driveTicksTwo;
                i++;
                newDiff = outerArray[i+2] - outerArray[i+1];
            }
            driveForward(totalDist);
            adjust();
            turnRight();
        }
        else //next square is below the current square
        {
            int newDiff = outerArray[i+2] - outerArray[i+1];
            while (newDiff==4 && i<6)
            {
                totalDist+= driveTicksTwo;
                i++;
                newDiff = outerArray[i+2] - outerArray[i+1];
            }
            driveForward(-totalDist);
            adjustFront();
            adjust();
        }
        i++;
    }

    for (int j=0; j<2; j++) //blink three times
    {
      high(26);
      pause(1000);
      low(26);
      pause(1000);
    }      
    
}
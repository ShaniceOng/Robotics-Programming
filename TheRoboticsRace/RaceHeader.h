#ifndef RACEHEADER_H_INCLUDED
#define RACEHEADER_H_INCLUDED

void turnRight(); //function for turning right with drive_goto
void turnLeft(); // function for turning left with drive_goto
void adjust(); //straightening the robot's position
int getNextIndex (int index, int currentNodeCount); //get index of the node the robot is going to
int getBackIndex (int index, int currentNodeCount); //get index of the node the robot is going back to
int obstacleCheck(); //get the distance from the wall using PING sensors
void adjustFront(); //adjust position of the robot (forward/backward)
void blink(); //blink robot LED three times
int calculateProportional(int target, int measured); //calculate proportional value for PID
void turn(int difference); //turn function for PID
void driveForward (int distance); // driving a number of ticks with PID

#endif
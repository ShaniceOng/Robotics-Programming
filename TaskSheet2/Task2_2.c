/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"  
#include "abdrive.h" 
#include <stdio.h>                   
#include <stdbool.h>
#include <math.h>

int arraysIndex;
int left[20];
int right[20];
int leftPosition[20];

//Bubble Sort function with input array and size of array as parameters
void bubbleSort(int *array, long unsigned n)
{

    long unsigned size = n;
    int iterationCount = 1;
    int currentNum;

    bool switched = true;
    while (switched==true)
    {
        switched=false;
        for (int i=1; i<n; i++)
        {
            
            if (array[i]<array[i-1])
            {
                left[arraysIndex] = array[i-1];
                right[arraysIndex] = array[i];
                leftPosition[arraysIndex] = i-1;
                arraysIndex++;
                
                currentNum = array[i];
                array[i]=array[i-1];
                array[i-1]=currentNum;
                switched=true;
            }
            printf("Iteration %i: ", iterationCount);
            for (int j=0; j<size; j++)
            {
                printf("%i ", array[j]);
            }
            printf("\n");
            iterationCount++;
        }
        n-=1;
    }

}

int main(void) {
    //Get user input
    printf("Enter number of elements: ");
    int arraySize;
    scanf("%i", &arraySize);
    int toSort[arraySize];
    for (int i=0; i<arraySize; i++)
    {
        printf("Enter element %i: ", i+1);
        scanf("%i", &toSort[i]);
    }
    int num = arraySize;
    
    //Sort the array
    bubbleSort(toSort, arraySize);
    
    //print final array
    for (int j=0; j<num; j++)
    {
        printf("%i, ", toSort[j]);
    }

    
}



#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
  
void insertionSort(int arr[], int n); 
void merge(int a[], int l1, int h1, int h2); 

void mergeSort(int a[], int l, int h) 
{
    int i, len=(h-l+1); 
    if (len<=5) 
    { 
        insertionSort(a+l, len); 
        return; 
    }  
}
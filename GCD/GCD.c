#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <x86intrin.h>

unsigned int Find_GCD(unsigned int a,unsigned int b)
{
    
    unsigned int c=1;
    while(c>0)
    {
        c=a%b;
        if(c==0)
        {
            return b;
        }
        a=b;
        b=c;
    }
    return b;
}


int main()
{
    srand(time(NULL));

    unsigned int itr=1000000;
    volatile unsigned int result = 0;
    unsigned int aux;
    unsigned long long start_cycles, end_cycles;  
    clock_t start_time, end_time;

    unsigned int *A = malloc(itr * sizeof(unsigned int));
    unsigned int *B = malloc(itr * sizeof(unsigned int));


    for(int i=0;i<itr;i++)
    {
        A[i]=rand()+1;
        B[i]=rand()+1;
    }

    start_time = clock();
    start_cycles = __rdtscp(&aux);
    for(int i=0;i<itr;i++)
    {
        result=Find_GCD(A[i],B[i]);
    }
    end_cycles = __rdtscp(&aux);
    end_time = clock();

   double total_time =
        ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    double average_time =
        total_time / itr;

    double average_cycles =
        (double)(end_cycles - start_cycles) / itr;
    
   printf("Number of iterations : %u\n", itr);
   printf("Total execution time : %f s\n", total_time);
   printf("Average Time per GCD : %.3f ns\n", average_time * 1e9);
   printf("average cycles per GCD : %.3f cycles\n", average_cycles);
    
    
   
   return 0;
}
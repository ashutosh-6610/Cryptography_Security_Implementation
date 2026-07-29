#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int Find_GCD(int a,int b)
{
    if(a<b)
    {
        int c=a;
        a=b;
        b=c;
    }
    int c=1;
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

    int itr=1000000;
    int *A = malloc(itr * sizeof(int));
    int *B = malloc(itr * sizeof(int));
    for(int i=0;i<itr;i++)
    {
        A[i]=rand();
        B[i]=rand();
    }

    clock_t start = clock();
    for(int i=0;i<itr;i++)
    {
        Find_GCD(A[i],B[i]);
    }
   clock_t end = clock();

   double total_time = (double)(end - start) / CLOCKS_PER_SEC;
   double average_time = total_time / itr;
   printf("Number of iterations : %d\n", itr);
   printf("Total execution time : %f seconds\n", total_time);
   printf("Average time/GCD     : %f seconds\n", average_time);

    return 0;
}
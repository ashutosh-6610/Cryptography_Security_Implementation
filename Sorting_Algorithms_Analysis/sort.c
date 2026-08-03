#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Merge Sort */
unsigned long long mergeComparisons = 0;
unsigned long long mergeMoves = 0;

/* Quick Sort */
unsigned long long quickComparisons = 0;
unsigned long long quickSwaps = 0;

/* Heap Sort */
unsigned long long heapComparisons = 0;
unsigned long long heapSwaps = 0;


void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void generateRandomArray(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        arr[i] = rand();
    }
}

void copyArray(int source[], int destination[], int n)
{
    for(int i = 0; i < n; i++)
        destination[i] = source[i];
}

void resetCounters()
{
    mergeComparisons = 0;
    mergeMoves = 0;

    quickComparisons = 0;
    quickSwaps = 0;

    heapComparisons = 0;
    heapSwaps = 0;
}

double calculateC(unsigned long long comparisons, int n)
{
    return (double)comparisons / (n * log2((double)n));
}


void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    /* Copy left half */
    for(int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
        mergeMoves++;
    }

    /* Copy right half */
    for(int i = 0; i < n2; i++)
    {
        R[i] = arr[mid + 1 + i];
        mergeMoves++;
    }

    int i = 0;
    int j = 0;
    int k = left;

    /* Merge the two sorted halves */
    while(i < n1 && j < n2)
    {
        mergeComparisons++;

        if(L[i] <= R[j])
        {
            arr[k++] = L[i++];
            mergeMoves++;
        }
        else
        {
            arr[k++] = R[j++];
            mergeMoves++;
        }
    }

    /* Copy remaining elements of L[] */
    while(i < n1)
    {
        arr[k++] = L[i++];
        mergeMoves++;
    }

    /* Copy remaining elements of R[] */
    while(j < n2)
    {
        arr[k++] = R[j++];
        mergeMoves++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right)
{
    if(left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}



int partition(int arr[], int low, int high)
{
    int pivot = arr[high];

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        quickComparisons++;

        if(arr[j] <= pivot)
        {
            i++;

            quickSwaps++;
            swap(&arr[i], &arr[j]);
        }
    }

    quickSwaps++;
    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if(low < high)
    {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);

        quickSort(arr, pivotIndex + 1, high);
    }
}




void heapify(int arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    /* Compare left child */
    if(left < n)
    {
        heapComparisons++;

        if(arr[left] > arr[largest])
        {
            largest = left;
        }
    }

    /* Compare right child */
    if(right < n)
    {
        heapComparisons++;

        if(arr[right] > arr[largest])
        {
            largest = right;
        }
    }

    if(largest != i)
    {
        heapSwaps++;
        swap(&arr[i], &arr[largest]);

        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n)
{
    /* Build Max Heap */

    for(int i = n/2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    /* Extract one element at a time */

    for(int i = n-1; i > 0; i--)
    {
        heapSwaps++;
        swap(&arr[0], &arr[i]);

        heapify(arr, i, 0);
    }
}


int main()
{
    srand(time(NULL));

    int sizes[] = {500,750, 1000,2500, 5000,7500, 10000, 12000,15000,18000,25000,29000,34000};
    int testCases = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("sorting_results.csv", "w");

    if(fp == NULL)
    {
        printf("Error creating file!\n");
        return 1;
    }

    fprintf(fp,
            "Algorithm,n,Comparisons,SwapsOrMoves,Time(ms),C\n");

    clock_t start, end;

    for(int t = 0; t < testCases; t++)
    {
        int n = sizes[t];

        double nlogn = n * log2((double)n);

        int *original   = (int *)malloc(n * sizeof(int));
        int *mergeArray = (int *)malloc(n * sizeof(int));
        int *quickArray = (int *)malloc(n * sizeof(int));
        int *heapArray  = (int *)malloc(n * sizeof(int));

        if(original == NULL || mergeArray == NULL ||
           quickArray == NULL || heapArray == NULL)
        {
            printf("Memory Allocation Failed!\n");
            fclose(fp);
            return 1;
        }

        /* Generate Random Array */

        generateRandomArray(original, n);

        /* Copy Original Array */

        copyArray(original, mergeArray, n);
        copyArray(original, quickArray, n);
        copyArray(original, heapArray, n);

        /*==================================================
                        MERGE SORT
        ==================================================*/

        mergeComparisons = 0;
        mergeMoves = 0;

        start = clock();

        mergeSort(mergeArray, 0, n - 1);

        end = clock();

        double mergeTime =
            (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        double mergeC =
            mergeComparisons / nlogn;

        fprintf(fp,
                "Merge,%d,%llu,%llu,%.6f,%.6f\n",
                n,
                mergeComparisons,
                mergeMoves,
                mergeTime,
                mergeC);

        /*==================================================
                        QUICK SORT
        ==================================================*/

        quickComparisons = 0;
        quickSwaps = 0;

        start = clock();

        quickSort(quickArray, 0, n - 1);

        end = clock();

        double quickTime =
            (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        double quickC =
            quickComparisons / nlogn;

        fprintf(fp,
                "Quick,%d,%llu,%llu,%.6f,%.6f\n",
                n,
                quickComparisons,
                quickSwaps,
                quickTime,
                quickC);

        /*==================================================
                        HEAP SORT
        ==================================================*/

        heapComparisons = 0;
        heapSwaps = 0;

        start = clock();

        heapSort(heapArray, n);

        end = clock();

        double heapTime =
            (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        double heapC =
            heapComparisons / nlogn;

        fprintf(fp,
                "Heap,%d,%llu,%llu,%.6f,%.6f\n",
                n,
                heapComparisons,
                heapSwaps,
                heapTime,
                heapC);

        free(original);
        free(mergeArray);
        free(quickArray);
        free(heapArray);
    }

    fclose(fp);

    printf("Results saved to sorting_results.csv\n");

    return 0;
}

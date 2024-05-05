#include <omp.h>
#include <stdio.h>
#include <time.h>

int a2_2() {

    // defining the array size
    const int arraySize = 1000000;
    int a[arraySize];

    // set the number of threads being used
    int threadsUsed = 1;

    // start measuring time
    const clock_t begin = clock();

    // each array element gets the value 1 assigned
#pragma omp parallel for num_threads(threadsUsed)
    for (int i = 0; i < arraySize; i++) {
        a[i] = 1;
    }

    // stop measuring time
    clock_t end = clock();

    // calculate and display execution time for the parallel part
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);

    return 0;
}

// calling the function
int main() {
   a2_2();
   return 0;
}
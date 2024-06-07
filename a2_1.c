#include <omp.h>
#include <stdio.h>
#include <time.h>

int a2_1() {

    // defining the array size
    const int arraySize = 1000000;
    int a[arraySize];

    // start measuring time
    const clock_t begin = clock();

    // each array element gets the value 1 assigned
#pragma omp parallel for num_threads(1)
    for (int i = 0; i < arraySize; i++) {
        a[i] = 1;
    }

    // stop measuring time
    clock_t end = clock();

    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);

    return 0;
}

/*
int main() {
    a2_1();
    return 0;
}
*/

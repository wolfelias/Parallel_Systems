#include <omp.h>
#include <stdio.h>
#include <time.h>

int a2_2() {
    const clock_t begin = clock();

    const int arrayLength = 1000000;
    int a[arrayLength];

#pragma omp parallel for num_threads(1)
    for (int i = 1; i <= arrayLength; i++) {
        a[i] = 1;
    }

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);

    return 0;
}

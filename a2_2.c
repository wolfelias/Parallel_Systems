#include <omp.h>
#include <stdio.h>

// applying fibonacci with only one thread
int sequential_fib(long long int n) {
    printf("Thread number: %d - Parameter: %d \n", omp_get_thread_num(), n);
    long long i, j;

    if (n < 2)
        return n;
    else {
        i = sequential_fib(n - 1);
        j = sequential_fib(n - 2);
        return i + j;
    }
}

// applying fibonacci with numerous threads
long long parallel_fib(long long n, int num_threads) {
    // switch back to sequential fibonacci if there is only one thread
    if (num_threads == 1) {
        return sequential_fib(n);
    }

    // divide the total number of threads into two different parts
    int threadsFori, threadsForj;

    // case for odd number of threads
    if ((num_threads % 2) == 1) {
        // currently, i gets one more thread for its calculations
        threadsFori = num_threads / 2 + 1;
        threadsForj = num_threads / 2;
    }
    // case for even number of threads – equally distributed
    else {
        threadsFori = num_threads / 2;
        threadsForj = num_threads / 2;
    }

    long long i, j;

    // the formula for parallel fibonacci
    if (n < 2)
        return n;
    else {
#pragma omp parallel sections num_threads(num_threads)
        {
#pragma omp section
            i = parallel_fib(n - 1, threadsFori);
#pragma omp section
            j = parallel_fib(n - 2, threadsForj);
        }
    }
    return i + j;
}

void main() {
    omp_set_nested(1);
    int n = 10;
    int num_threads = 20;
    printf("fib(%d) = %lli\n", n, parallel_fib(n, num_threads));
}

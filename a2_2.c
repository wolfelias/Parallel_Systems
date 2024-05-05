#include <omp.h>
#include <stdio.h>

// applying fibonacci with only one thread
int sequential_fib(long long int n) {
    printf("Last thread: %d\n", omp_get_thread_num());
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

    int threads_left;
    if ((num_threads % 2) == 1)
        threads_left = num_threads / 2 + 1;
    else
        threads_left = num_threads / 2;


    printf("Num threads left: %d\n", threads_left);

    long long i, j;
    if (n < 2)
        return n;
    else {
#pragma omp parallel sections num_threads(threads_left)
        {
#pragma omp section
            i = parallel_fib(n - 1, threads_left);
#pragma omp section
            j = parallel_fib(n - 2, threads_left);
        }
    }
    return i + j;
}

int main() {
    int n = 10;
    int num_threads = 8;
    printf("fib(%d) = %d\n", n, parallel_fib(n, num_threads));
    return 0;
}

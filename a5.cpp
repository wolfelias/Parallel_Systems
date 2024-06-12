#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <pthread.h>
#include <mach/mach.h>
#include <mach/thread_policy.h>


// Struct used for storing data
struct data {
    double stride;
    double arraySize;
    double time_per_access;
};

data cache_test(int array_size, int stride) {
    int iterations = 10000;
    // Set values of array to 2
    int *array = static_cast<int *>(malloc(array_size * sizeof(int)));
    auto temp = 0;
    // Start clock
    timespec start{}, end{};
    clock_gettime(CLOCK_REALTIME, &start);

    for (volatile int times = 0; times <= iterations; ++times) {
        for (volatile int i = 0; i < array_size; i += stride) {
            volatile char value = array[i];
        }
    }

    // Stop clock and calc time
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
    double time_per_array_iteration = elapsed_time / iterations;
    double accesses_per_array = (array_size / stride);
    double time_per_access = time_per_array_iteration / accesses_per_array;

    data dataStruct;
    dataStruct.stride = stride;
    dataStruct.time_per_access = time_per_access;
    dataStruct.arraySize = array_size;

    free(array);

    return dataStruct;
}

int writeToFile(const std::list<data> *d) {
    char filename[] = "data.csv";
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return -1;
    }
    file << "Stride" << "," << " Arraysize" << "," << " Time per Access" << "\n";

    for (data i: *d) {
        file << i.stride << "," << i.arraySize << "," << i.time_per_access << "\n";
    }
    file.close();
    return 0;
}

void setCPUAffinity(int cpu) {
    thread_affinity_policy_data_t policy = {cpu};

    if (const thread_port_t mach_thread = pthread_mach_thread_np(pthread_self()); thread_policy_set(mach_thread,
        THREAD_AFFINITY_POLICY, reinterpret_cast<thread_policy_t>(&policy),
        THREAD_AFFINITY_POLICY_COUNT) != KERN_SUCCESS) {
        std::cerr << "Error: Could not set cpu affinity" << std::endl;
    }
}

int main() {
    // Set cpu affinity to 1 core
    setCPUAffinity(0);

    std::vector<size_t> array_sizes = {
        1 << 12, // 4 KB = 4 * 1024
        1 << 13, // 8 KB = 8 * 1024
        1 << 14, // 16 KB = 16 * 1024
        1 << 15, // 32 KB = 32 * 1024
        1 << 16, // 64 KB = 64 * 1024
        1 << 17, // 128 KB = 128 * 1024
        1 << 18, // 256 KB = 256 * 1024
        1 << 19, // 512 KB = 512 * 1024
        1 << 20, // 1 MB = 1024 * 1024
        1 << 21, // 2 MB = 2 * 1024 * 1024
        1 << 22, // 4 MB = 4 * 1024 * 1024
        1 << 23, // 8 MB = 8 * 1024 * 1024
        1 << 24, // 16 MB = 16 * 1024 * 1024
        1 << 25 // 32 MB = 32 * 1024 * 1024
    };

    int strides[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

    std::list<data> results = {};

    for (int i: strides) {
        results.push_back(cache_test(8000, i));
        results.push_back(cache_test(16000, i));
        results.push_back(cache_test(32000, i));
        results.push_back(cache_test(64000, i));
    }

    int result = writeToFile(&results);
    if (result == 0) {
        printf("Wrote results to file :)");
    } else {
        printf("Error writing results :(");
    }
    return 0;
}

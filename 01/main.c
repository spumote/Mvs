#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <assert.h>


typedef struct {
    int finishes_in_b;
    int time;
} results_t;

results_t RandomWalk(int a, int b, int x, double p, unsigned int *seed) {
    results_t result = {
        .finishes_in_b = 0,
        .time = 0,
    };
    int pMax = (p * RAND_MAX);
    int cur_pos = x;
    while (1) {
        int step;
        if (rand_r(seed) < pMax)
            cur_pos += 1;
        else
            cur_pos -= 1;
        result.time++;
        if (cur_pos == b) {
            result.finishes_in_b++;
            break;
        }
        if (cur_pos == a)
            break;
    }
    return result;
}

int main(int argc, const char* argv[]) {
    assert(argc == 7);
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int x = atoi(argv[3]);
    int N = atoi(argv[4]);
    double p = atof(argv[5]);
    int P = atoi(argv[6]);
    assert(b > a);
    assert(x > a && x < b);

    omp_set_num_threads(P);
    srand(time(NULL));
    
    int all_finishes_in_b = 0;
    int all_time = 0;
    double start_time = omp_get_wtime();
    unsigned int seed;
    #pragma omp parallel for private(seed) reduction(+: all_time, all_finishes_in_b)
        for (int i = 0; i < N; ++i) {
            seed = rand();
            results_t result = RandomWalk(a, b, x, p, &seed);
            all_time += result.time;
            all_finishes_in_b += result.finishes_in_b;
        }
    double end_time = omp_get_wtime();
    double b_probability = (double)all_finishes_in_b / N;
    double average_time = (double)all_time / N;

    FILE * file;
    file = fopen("stats.txt", "ab");
    assert(file != NULL);
    fprintf(file, "%f, %f, %fs, %d, %d, %d, %d, %f, %d\n", b_probability, average_time, end_time - start_time, a, b, x, N, p, P);
    fclose(file);
    
    return 0;
}

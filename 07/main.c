#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    int r;
    int x;
    int y;
} particle;

typedef struct {
    int rank;
    int size;
    int l;
    int a;
    int b;
    int N;
} task_data;

void RandomWalk(task_data* d) {
    double start = MPI_Wtime();

    particle *particles = (particle*)malloc(d->N * sizeof(particle));
    int seed;
    int *seeds = (int*)malloc(d->size * sizeof(int));

    if (d->rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < d->size; i++) {
            seeds[i] = rand();
        }
    }

    MPI_Scatter(seeds, 1, MPI_UNSIGNED, &seed, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    for (int i = 0; i < d->N; i++) {
        particles[i].r = rand_r(&seed) % (d->a * d->b);
        particles[i].x = rand_r(&seed) % d->l;
        particles[i].y = rand_r(&seed) % d->l;
    }

    int squareSize = d->l * d->l * d->size;
    int *squares = (int*)malloc(squareSize * sizeof(int));
    for (int i = 0; i < squareSize; i++) 
        squares[i] = 0;

    MPI_File_delete("data.bin", MPI_INFO_NULL);
    MPI_File binaryFile;
    MPI_File_open(MPI_COMM_WORLD, "data.bin", MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &binaryFile);

    for (int i = 0; i < d->N; i++) {
        int x = particles[i].x;
        int y = particles[i].y;
        int r = particles[i].r;
        squares[y * d->l * d->size + x * d->size + r]++;
    }

    MPI_Datatype view;
    MPI_Type_vector(d->l, d->l * d->size, d->l * d->a * d->size, MPI_INT, &view);
    MPI_Type_commit(&view);

    int xCoord = d->rank % d->a;
    int yCoord = d->rank / d->a;
    int offset = (xCoord * d->l + yCoord * d->a * d->l * d->l) * d->size;

    MPI_File_set_view(binaryFile, offset * sizeof(int), MPI_INT, view, "native", MPI_INFO_NULL);

    MPI_File_write(binaryFile, squares, squareSize, MPI_INT, MPI_STATUS_IGNORE);
    MPI_Type_free(&view);

    MPI_File_close(&binaryFile);

    double finish = MPI_Wtime();
    if (d->rank == 0) {
        FILE *file = fopen("stats.txt", "w");
        fprintf(file, "%d %d %d %d %fs\n", d->l, d->a, d->b, d->N, finish - start);
        fclose(file);
    }

    free(seeds);
    free(squares);
    free(particles);
}

int main(int argc, char * argv[]) {
    assert(argc == 5);
    int l = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    int N = atoi(argv[4]);

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    task_data d = {
        .l = l,
        .a = a,
        .b = b,
        .N = N,
        .rank = rank,
        .size = size,
    };
    RandomWalk(&d);

    MPI_Finalize();
    return 0;
}

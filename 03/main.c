#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>


struct timeval tv1, tv2, dtv;
struct timezone tz;
int n;
int P;
int m;

typedef struct {
    int l1;
    int r1;
    int l2;
    int r2;
    int* arr;
} pthread_data_t;

typedef struct {
    int start_index;
    int number_of_chunks_for_thread;
    int* chunks_arr;
} sorting_data_t;

int cmp(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void TimeStart() { 
    gettimeofday(&tv1, &tz); 
}

double TimeStop() {
    gettimeofday(&tv2, &tz);
    dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
    dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
    if (dtv.tv_usec < 0) { 
        dtv.tv_sec--; 
        dtv.tv_usec += 1000000; 
    }
    return dtv.tv_sec + (float)dtv.tv_usec / 1000000;
}

int BinarySearch(int* arr, int key, int left, int right) {
    if (arr[left] > key)
        return left;
    else if (arr[right] < key)
        return right + 1;
    else
        right;
   
    right++;
    while (right - left > 1) {
        int md = (left + right) / 2;

        if (arr[md] == key)
            return md;
        else if (arr[md] > key)
            right = md;
        else
            left = md;
    }

    return left;
}

void* ThreadSorting(void* sorting_data_) {
    sorting_data_t* sorting_data = (sorting_data_t*) sorting_data_;
    int start_index = sorting_data->start_index;
    int number_of_chunks_for_thread = sorting_data->number_of_chunks_for_thread;
    int* chunks_arr = sorting_data->chunks_arr;

    int cur_index = start_index;
    for (int i = 0; i < number_of_chunks_for_thread; i++) {
        qsort(&chunks_arr[cur_index], m, sizeof(int), cmp);
        cur_index += m;
    }
    return NULL;
}

void* Merge(void* data_) {
    pthread_data_t* data = (pthread_data_t*) data_;
    int l1 = data->l1;
    int r1 = data->r1;
    int l2 = data->l2;
    int r2 = data->r2;
    int* arr = data->arr;

    int i = 0;
    int j = 0;
    int* temp1 = malloc((r1 - l1 + 1) * sizeof(int));
    int* temp2 = malloc((r2 - l2 + 1) * sizeof(int));


    memcpy(temp1, &arr[l1], (r1 - l1 + 1) * sizeof(int));
    memcpy(temp2, &arr[l2], (r2 - l2 + 1) * sizeof(int));

    while ((i < (r1 - l1 + 1)) && (j < (r2 - l2 + 1))) {
        if (temp1[i] < temp2[j]) {
            arr[l1 + i + j] = temp1[i];
            i++;
        } else {
            arr[l1 + i + j] = temp2[j];
            j++;
        }
    }
    while (i < (r1 - l1 + 1)) {
        arr[l1 + i + j] = temp1[i];
        i++;
    }
    while (j < (r2 - l2 + 1)) {
        arr[l1 + i + j] = temp2[j];
        j++;
    }

    free(temp1);
    free(temp2);
    return NULL;
}

void InitialChunksSorting(int* arr, int n) {
    pthread_t* threads = (pthread_t*) malloc(P * sizeof(pthread_t));
    sorting_data_t* thread_data = malloc(P * sizeof(sorting_data_t));

    int number_of_chunks = n / m;
    int remain = n % m;
    int number_of_chunks_for_thread = number_of_chunks / P;

    int cur_index = 0;

    for (int i = 0; i < P; i++) {
        thread_data[i].start_index = cur_index;
        thread_data[i].number_of_chunks_for_thread = number_of_chunks_for_thread;
        thread_data[i].chunks_arr = arr;
        pthread_create(&(threads[i]), NULL, ThreadSorting, &thread_data[i]);
        cur_index += m * number_of_chunks_for_thread;
    }
    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }
    if (remain != 0 || number_of_chunks == 1)
        qsort(&arr[cur_index], n - cur_index, sizeof(int), cmp);
    free(threads);
    free(thread_data);
}

void ParallelMergeSort(int* arr, int n) {
    InitialChunksSorting(arr, n);
    pthread_t* threads = malloc(P * sizeof(pthread_t));
    pthread_data_t* thread_data = malloc(P * sizeof(pthread_data_t));

    for (int cur_size = m; cur_size < n; cur_size *= 2) {
        int cur_index = 0;
        int num_threads = P;
        while (cur_index + cur_size + 1 < n) {
            for (int i = 0; i < P; i++) {
                int l1 = cur_index;
                int r1 = cur_index + cur_size - 1;
                if (r1 >= n) {
                    num_threads = i;
                    break;
                }
                int l2 = r1 + 1;
                int r2 = l2 + cur_size - 1;
                if (r2 > n - 1) {
                    r2 = n - 1;
                }
                cur_index = r2 + 1;

                thread_data[i].l1 = l1;
                thread_data[i].r1 = r1;
                thread_data[i].l2 = l2;
                thread_data[i].r2 = r2;
                thread_data[i].arr = arr;

                pthread_create(&(threads[i]), NULL, Merge, &thread_data[i]);
            }

            for (int i = 0; i < num_threads; i++)
                pthread_join(threads[i], NULL);
        }
    }

    free(threads);
    free(thread_data);
}

int main(int argc, char** argv) {
    assert(argc == 4);
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    P = atoi(argv[3]);

    int* quick_sort_arr = (int*) malloc(sizeof(int) * n);
    int* merge_sort_arr = (int*) malloc(sizeof(int) * n);
    
    srand(time(NULL));
    for (int i = 0; i < n ; i++) {
        int t = rand() % n;
        merge_sort_arr[i] = t;
        quick_sort_arr[i] = t;
    }
    
    FILE *file1, *file2;
    file2 = fopen("data.txt", "w");
    for (int i = 0; i < n; i++)
        fprintf(file2, "%d ", merge_sort_arr[i]);
    fprintf(file2, "\n");
    
    TimeStart();
    ParallelMergeSort(merge_sort_arr, n);
    double merge_sort_time = TimeStop();
    printf("MergeSort time: %f\n", TimeStop());
    
    file1 = fopen("stats.txt", "ab");
    fprintf(file1, "%f %d %d %d\n", merge_sort_time, n, m, P);
    for (int i = 0; i < n; i++)
        fprintf(file2, "%d ", merge_sort_arr[i]);
    
    TimeStart();
    qsort(&quick_sort_arr[0], n, sizeof(int), cmp);
    printf("QuickSort time: %f\n", TimeStop());
    
    fclose(file1);
    fclose(file2);
    free(merge_sort_arr);
    free(quick_sort_arr);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned long long start, end;
} Interval;

int cmp_intervals(const void *a, const void *b){
    const Interval *A = a, *B = b;
    if (A->start < B->start) return -1;
    if (A->start > B->start) return 1;
    if (A->end < B->end) return -1;
    if (A->end > B->end) return 1;
    return 0;
}

int is_fresh_ingredient(Interval *arr, int m, unsigned long long x){
    if (m == 0) return 0;

    size_t lo = 0, hi = m - 1;

    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;

        if (arr[mid].start <= x) {
            if (arr[mid].end >= x) {
                printf("Id: %llu in range %llu - %llu\n",
                x, arr[mid].start, arr[mid].end);
                return 1;
            }
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return 0;
}

unsigned long long num_of_ids_in_interval(Interval *in){
    return in->end - in->start + 1;
}


int main(int argc, char* argv[]){
    FILE* fp;

    if(argc < 2){
        perror("Pls add txt file uwu");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    
    if(fp == NULL){
        perror("Error opening file");
        exit(1);
    }

    Interval *intervals = NULL;
    size_t count = 0;
    size_t capacity = 0;

    unsigned long long a, b;

    char buff[255];

    // read in ranges till blank line
    while(fgets(buff, sizeof buff, fp)){
   
        if(sscanf(buff, "%llu-%llu", &a, &b) == 2){
            // dynamic interval array reallocation
            if(count == capacity){
                capacity = capacity ? capacity * 2 : 8;
                intervals = realloc(intervals, capacity * sizeof(Interval));
                if(!intervals){
                    perror("relloc error");
                    exit(1);
                }

            }

        intervals[count].start = a;
        intervals[count].end = b;
        count++;

        } else {
            break;
        }
    }

    qsort(intervals, count, sizeof(Interval), cmp_intervals);

    // merge overlapping intervals
    Interval *merged = malloc(count * sizeof(Interval));
    size_t m = 0;

    for(int i = 0; i < count; i++){
        if(m == 0 || intervals[i].start > merged[m-1].end + 1){
            merged[m++] = intervals[i];
        } else if(intervals[i].end > merged[m-1].end) {
            merged[m-1].end = intervals[i].end;
        }
    }

    int res = 0;
    char *end;

    // calc num of fresh ingredients
    while(fgets(buff, sizeof buff, fp)){
        buff[strcspn(buff, "\n")] = 0;
        unsigned long long id = strtoull(buff, &end, 10);
        if(is_fresh_ingredient(merged, m, id)){
            res++;
        }
    }

    printf("Fresh ingredients: %d\n", res);
    
    // part2
    // calc num of total ids conssidered fresh
    unsigned long long tot = 0;
    unsigned long long sizeofrange = 0;
    for(int i = 0; i < m; i++){
        sizeofrange = num_of_ids_in_interval(&merged[i]);
        printf("Size of range %llu - %llu is %llu\n", merged[i].start, merged[i].end, sizeofrange);
        tot += sizeofrange;
    }

    printf("Num of toatlal resh ingredient ids: %llu\n", tot);
    
    free(intervals);
    fclose(fp);
    return 0;
}
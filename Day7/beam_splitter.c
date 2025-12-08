#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_splits(char **matrix, int rows, int cols, int x, int y){
    if(rows == y) return 0;
    if(matrix[y][x] == '|') return 0;
    if(x < 0 || x >= cols) return 0;
    if(matrix[y][x] == '^') {
        int splits = 1;
        splits += count_splits(matrix, rows, cols, x-1, y+1);
        return splits + count_splits(matrix, rows, cols, x+1, y+1);
    }
    matrix[y][x] = '|';
    return count_splits(matrix, rows, cols, x, y+1);

}

unsigned long long **cach;

unsigned long long count_paths(char **matrix, int rows, int cols, int x, int y){
    if(y == rows) return 1;                     // end of a path       
    if(x < 0 || x >= cols) return 0;            // out of bounds
    //if(matrix[y][x] == '|') return 0;           // Blocked cell

    // Return cached value if computed
    if (cach[y][x] != (unsigned long long)-1) return cach[y][x];

    unsigned long long paths = 0;

    if(matrix[y][x] == '^') {
       paths = count_paths(matrix, rows, cols, x - 1, y + 1) +
                count_paths(matrix, rows, cols, x + 1, y + 1);
        
    } else {
        paths = count_paths(matrix, rows, cols, x, y + 1);
    }

    cach[y][x] = paths;                         // cach results
    return paths;
}

int getLine(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (!*lineptr) return -1;
    }

    size_t i = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {

        if (c == '\n') {    // dont store new line character
            break;
        }

        if (i + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (!new_ptr) return -1;
            *lineptr = new_ptr;
            *n = new_size;
        }

        (*lineptr)[i++] = c;
    }

    if (i == 0 && c == EOF)
        return -1;

    (*lineptr)[i] = '\0';
    return i;
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

    // create a matrix 
    char *line = NULL;
    size_t len = 0;
    int  read;

    int rows = 0, cols = 0;

    while ((read = getLine(&line, &len, fp)) != -1){
        rows++;
        if(cols < read) cols = read;
    }

    char **matrix = malloc(rows * sizeof(char *));
    for(int i = 0; i < rows; i++){
        matrix[i] = malloc(cols + 1);
    }

    rewind(fp);
    int row = 0;
    while((read = getLine(&line, &len, fp)) != -1){
        for(int col = 0; col < read && col < cols; col++){
            matrix[row][col] = line[col];
        }
        row++;
    }
    free(line);
    fclose(fp);

    char *S = strchr(matrix[0], 'S');
    int start_x = (int)(S - matrix[0]);

    cach = malloc(rows * sizeof(unsigned long long *));
    for(int i = 0; i < rows; i++){
        cach[i] = malloc(cols * sizeof(unsigned long long));
        for(int j = 0; j < cols; j++)
            cach[i][j] = (unsigned long long)-1;
    }

    long res = 0;
    unsigned long long num_paths = 0;

    res += count_splits(matrix, rows, cols, start_x, 1);
    num_paths += count_paths(matrix, rows, cols, start_x, 1);

    printf("Num of splits = %ld\n", res);
    printf("Num of possible paths = %llu\n", num_paths);

    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
        free(cach[i]);
    }
    free(matrix);
    free(cach);

    return 0;
}
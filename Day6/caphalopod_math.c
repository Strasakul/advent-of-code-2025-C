#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned long long calculate_problem(char ***matrix, int rows, int problem){
    char operator = matrix[rows-1][problem][0];
    unsigned long long res = atoi(matrix[0][problem]);
    unsigned long long num = 0;
    for(int i = 1; i < rows-1; ++i){
        num = atoi(matrix[i][problem]);
        switch(operator){
            case '+':
                res += num;
                break;
            case '*':
                res *= num;
                break;
            default:
                perror("Operator not defined\n");
        }
    }
    return res;
}

int is_blank_line(char *str){
    int len = strlen(str);
    int count = 0;
    for(int i = 0; i < len; i++){
        if(isspace(str[i])) count++;
    }
    if(len == count) return 1;
    return 0;
}

// Windows strikes again
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

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc < 2)
    {
        perror("Pls add txt file uwu");
        exit(1);
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    int  read;

    int rows = 0, cols = 0;

    // count rows and cols
    while ((read = getLine(&line, &len, fp)) != -1){
        rows++;
        if(cols < read) cols = read;
    }
    printf("Rows: %d\n", rows);
    printf("Cols: %d\n", cols);

    // make matrix (transposed)
    char **matrix = malloc(cols * sizeof(char *));
    for(int i = 0; i < cols; i++){
        matrix[i] = malloc(rows + 1);
    }

    // fill matrix (transposed)
    rewind(fp);
    int row = 0;
    while((read = getLine(&line, &len, fp)) != -1){
        for(int col = 0; col < read && col < cols; col++){
            matrix[col][row] = line[col];
        }
        row++;
    }

    // null-terminate each row
    for (int i = 0; i < cols; i++) {
        matrix[i][rows] = '\0';
    }

    fclose(fp);

    unsigned long long res = 0;
    unsigned long long tmp = 0;
    int operator = 0;

    for(int i = 0; i < cols; ++i){
        char *str = matrix[i];
        if(is_blank_line(str)) {
            printf("tmp = %llu\n", tmp);
            res += tmp;
            tmp = 0;
            operator = 0;
            continue;
        }

        if(operator == 0){
            operator = matrix[i][rows-1];
             tmp = atoi(str);
             continue;
        }
        
        switch(operator){
            case '+':
                printf("%llu + %d\n", tmp, atoi(str));
                tmp += atoi(str);
                break;
            case '*':
                printf("%llu * %d\n", tmp, atoi(str));
                tmp *= atoi(str);
                break;
            default:
                break;
        }
    }

    res += tmp;

    printf("Total amount = %llu\n", res);

    free(line);
    free(matrix);

    return 0;
}
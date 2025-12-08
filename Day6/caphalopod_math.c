#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // get first line for column count
    if ((read = getLine(&line, &len, fp)) != -1) {
        rows++;
        char *tmp = strdup(line);
        char *tok = strtok(tmp, " \t\n");
        while (tok) {
            cols++;
            tok = strtok(NULL, " \t\n");
        }
        free(tmp);
    }
    printf("Cols: %d\n", cols);

    // count remaining rows
    while ((read = getLine(&line, &len, fp)) != -1){
        rows++;
    }
    printf("Rows: %d\n", rows);

    // make matrix
    char ***matrix = malloc(rows * sizeof(char **));
    for(int i = 0; i < rows; i++){
        matrix[i] = malloc(cols * sizeof(char *));
        for(int j = 0; j < cols; j++){
            matrix[i][j] = malloc(16); // max length of a token
        }
    }

    // fill matrix
    rewind(fp);
    int i = 0;
    while((read = getLine(&line, &len, fp)) != -1){

        char *tmp = strdup(line);
        char *tok = strtok(tmp, " ");
        int j = 0;
        while(tok && j < cols){         // dont count \n col
            strcpy(matrix[i][j], tok);
            tok = strtok(NULL, " ");
            j++;
        }
        i++;
    }

    // print matrix
    //for(int r = 0; r < rows; r++){
    //    for(int c = 0; c < cols; c++){
    //        printf("%s ", matrix[r][c]);
    //    }
    //    printf("\n");
    //}

    unsigned long long res = 0;
    unsigned long long tmp = 0;
    for(int i = 0; i < cols; ++i){
        tmp = calculate_problem(matrix, rows, i);
        printf("tmp = %llu\n", tmp);
        res += tmp;
    }

    printf("Total amount = %llu\n", res);

    free(line);
    free(matrix);
    fclose(fp);

    return 0;
}
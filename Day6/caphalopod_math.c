#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    int rows = 0, cols = 0;
    char buff[255];

    // num of cols
    fgets(buff, sizeof buff, fp);
    buff[strcspn(buff, "\n")] = 0;
    char *ptr = strtok(buff, " ");
    while(ptr != NULL){
        cols++;
        ptr = strtok(NULL, " ");
    }

    // num of rows
    rewind(fp);
    while(fgets(buff, sizeof buff, fp)){
        rows++;
    }

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
    while(fgets(buff, sizeof buff, fp)){
        buff[strcspn(buff, "\n")] = 0;
        char *ptr = strtok(buff, " ");
        int j = 0;
        while(ptr != NULL){
            strcpy(matrix[i][j], ptr);
            ptr = strtok(NULL, " ");
            j++;
        }
        i++;
    }

    // print matrix
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            printf("%s ", matrix[r][c]);
        }
        printf("\n");
    }

    return 0;
}
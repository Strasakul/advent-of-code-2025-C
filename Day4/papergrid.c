#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumOfNeighbors(char **matrix, int rows, int cols, int r, int c){
    int out = 0;
    //directions
    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int i = 0; i < 8; i++){
        int nr = r + dr[i];
        int nc = c + dc[i];

        if(nr >= 0 && nr < rows && nc >= 0 && nc < cols){
            if(matrix[nr][nc] == '@' || matrix[nr][nc] == 'X'){
                out++;
            }
        }
    }

    return out;
}

int removeFreeRolls(char **matrix, int rows){
    int out = 0;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < strlen(matrix[i]); j++){
            if(matrix[i][j] == '.') continue;
            if(getNumOfNeighbors(matrix, rows, strlen(matrix[i]), i, j) < 4){
                matrix[i][j] = 'X';
                out++;
            }
        }
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < strlen(matrix[i]); j++){
            if(matrix[i][j] == 'X') matrix[i][j] = '.';
        }
        
    }

    return out;
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

    int lines = 0;
    int maxLen = 0;
    char buff[255];

    while(fgets(buff, sizeof(buff), fp)){
        lines++;
        int len = strlen(buff);
        if(len > maxLen) maxLen = len;
    }

    char **paperMatrix = malloc(lines * sizeof(char *));
    for(int i = 0; i < lines; i++){
        paperMatrix[i] = malloc(maxLen + 1);
    }

    rewind(fp);
    int i = 0;
    while(fgets(buff, sizeof buff, fp)){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(paperMatrix[i], buff);
        i++;
    }

    fclose(fp);

    int res = 0;
    int numOfRolls = removeFreeRolls(paperMatrix, lines);
    while(numOfRolls > 0){
        res += numOfRolls;
        printf("Removed %d rolls\n", res);
        numOfRolls = removeFreeRolls(paperMatrix, lines);
    }

    free(paperMatrix);

    printf("Num of removable paper Rolls: %d", res);

    return 0;
}
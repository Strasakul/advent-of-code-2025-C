#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getLargest(const char* line, char* out, int numberOfDigits) {
    int len = strlen(line);
    int start = 0;
    
    for(int i = 0; i < numberOfDigits; ++i){
        int end = len - (numberOfDigits - i);
        char bestDigit = '0' - 1;
        int bestIndex = start;

        for(int j = start; j <= end; ++j){
            if(line[j] > bestDigit){
                bestDigit = line[j];
                bestIndex = j;
                if(bestDigit == '9') break;
            }
        }

        out[i] = bestDigit;
        start = bestIndex + 1;
    }

    out[numberOfDigits] = '\0';
}


int main(int argc, char* argv[]){
    FILE* fp;

    if(argc < 3){
        perror("Pls add txt file and number of digits uwu");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    
    if(fp == NULL){
        perror("Error opening file");
        exit(1);
    }

    char buff[255];
    char largest[255];
    unsigned long long res = 0;
    char* end;

    while (fgets(buff, sizeof(buff), fp) != NULL) {
        buff[strcspn(buff, "\n")] = 0;
        getLargest(buff, largest, atoi(argv[2]));
        res += strtoull(largest, &end, 10);
    }

    printf("Result: %llu\n", res);

    fclose(fp);
    return 0;
}
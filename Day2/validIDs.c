#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getbydelim(char* _Buffer, int _MaxCount, char _Delimiter, FILE *_Stream){
    int c;
    size_t i = 0;

    while(((c = fgetc(_Stream)) != EOF && c != _Delimiter)){
        if(i + 1 < _MaxCount){
            _Buffer[i++] = c;
        }
    }

    _Buffer[i] = '\0';

    return (c == EOF && i == 0) ? 0 : 1;
}

int isInvalidIdPart1(unsigned long long id){
    char idstr[255];
    sprintf(idstr, "%llu", id);
    int len = strlen(idstr);

    // must be even length to be a repeated sequence
    if((len % 2) != 0) return 0;

    int half = len/2;

    for(int i = 0; i < half; i++){
        if(idstr[i] != idstr[i + half]){
            return 0;
        }
    }
    return 1;
}

int isInvalidIdPart2(unsigned long long id){
    char idstr[255];
    sprintf(idstr, "%llu", id);
    int len = strlen(idstr);
    int half = len/2;

    for(int sizeOfPart = 1; sizeOfPart <= half; sizeOfPart++){
        if(len % sizeOfPart) continue;
        int j = 0;
        while(idstr[j] == idstr[j + sizeOfPart]){
            if((j + 1 + sizeOfPart) >= len){
                //printf("Found Invalid ID %s\n", idstr);
                return 1;
            }
            j++;
        }
    }


    return 0;
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

    char buff[255];
    unsigned long long baseId, endId, idCount = 0;

    while(getbydelim(buff, sizeof(buff), ',', fp)){
        if(sscanf(buff, "%llu-%llu", &baseId, &endId) == 2){
            for(unsigned long long id = baseId; id <= endId; id++){
                if(isInvalidIdPart2(id)){
                    idCount += id;
                }
            }

        }
    }

    printf("Total invalid ID count: %llu", idCount);

    fclose(fp);

    return 0;
}


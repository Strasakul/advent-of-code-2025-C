#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]){

    const int N = 100;
    FILE* fp;

    if(argc < 2){
        perror("Pls add txt file");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    
    if(fp == NULL){
        perror("Error opening file");
        exit(1);
    }
    char buf[256];
    int dial = 50;        // starting position
    int res = 0;          // count of times passing 0

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        char dir = 0;
        int count = 0;
        int wraps = 0;
        int newpos = 0;
        if (sscanf(buf, " %c%d", &dir, &count) == 2) {
            //if (dir == 'L') {
            //    count *= -1;
            //}
            //newpos = dial + count;
            //wraps = abs((int)floor(newpos / 100) - (int)floor(dial / 100));
//
            //dial = (newpos % 100 + 100) % 100;

            // gave up and just brute forced it lol

            for(int i = 0; i < count; i++){
                if(dir == 'R'){
                    dial++;;
                    if(dial > 99){
                        dial = 0;
                    }
                } else if(dir == 'L'){
                    dial--;
                    if(dial < 0){
                        dial = 99;
                    }
                }
                if(dial == 0){
                   res++;
                }
            }

        }

        
    }

    printf("Total passes over 0: %d\n", res);

    fclose(fp);
    return 0;

}
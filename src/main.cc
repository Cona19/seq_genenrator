#include <iostream>
#include <fstream>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"

char *dna;
char types[5] = "ACGT";

void *thread_main(void *arg){
    int tid = (long long)arg;
    long long x;
    std::ofstream out1(kFilePath + std::to_string(tid) + "-1.fastq");
    std::ofstream out2(kFilePath + std::to_string(tid) + "-2.fastq");
    for (long long i = 0; i <= kTotalLength - kStrLength; i++){
        if (i % 1000000 == 0){
            std::cout << "." << std::endl;
        }
        out1 << "@EAS139:136:FC706VJ:2:2104:15343:197393 1:Y:18:ATCACG" << std::endl; //Sample id
        out2 << "@EAS139:136:FC706VJ:2:2104:15343:197393 1:Y:18:ATCACG" << std::endl; //Sample id
        x = i;
        for (long long j = x; j < x + kStrLength; j++){
            if (rand()%1000 < kErrorRate * 1000){// && rand()%100 == 0){
                out1 << types[rand()&3];
                out2 << types[rand()&3];
            } else {
                out1 << dna[j];
                out2 << dna[j];
            }
        }
        out1 << std::endl;
        out2 << std::endl;
        out1 << "+" << std::endl;
        out2 << "+" << std::endl;
        for (long long j = 0; j < kStrLength; j++){
            out1 << "J";
            out2 << "J";
        }
        out1 << std::endl;
        out2 << std::endl;
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    std::ios::sync_with_stdio(false);
    srand((unsigned int) time(NULL));
    std::cout << kTotalLength << std::endl;
    dna = (char*)malloc(kTotalLength + 1);
    for (unsigned long long i = 0; i < kTotalLength; i++){
        dna[i] = types[rand()&3];
    }
    pthread_t threads[kNumThread];
    for (long long i = 0; i < kNumThread; i++){
        pthread_create(&threads[i], NULL, thread_main, (void*)i);
    }
    std::ofstream out("answer.txt");
    out << dna << std::endl;
    out.close();
    for (long long i = 0; i < kNumThread; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

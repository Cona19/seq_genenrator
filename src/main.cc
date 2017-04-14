#include <iostream>
#include <fstream>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "config.h"

char *dna;
char types[5] = "ACGT";
static const unsigned long long NUM_STR = CFG_TOTAL_LENGTH * CFG_NUM_DUP / CFG_STR_LENGTH / CFG_NUM_THREAD;

static const unsigned long long NUM_TOT_STR = CFG_TOTAL_LENGTH * CFG_NUM_DUP / CFG_STR_LENGTH;

void *thread_main(void *arg){
    int tid = (long long)arg;
    std::ostringstream ss;

    ss << CFG_FILEPATH << tid << ".txt";
    std::ofstream out(ss.str());
    unsigned long long end = NUM_STR + (NUM_STR * CFG_NUM_THREAD + tid < NUM_TOT_STR ? 1 : 0);
    for (unsigned long long i = 0; i < end; i++){
        if (i % 1000000 == 0){
            std::cout << ".";
        }
        out << i << std::endl;
        unsigned long long x = ((unsigned long long) rand() << 31) + rand();
        x %= CFG_TOTAL_LENGTH-CFG_STR_LENGTH;
        out.write(&dna[x], CFG_STR_LENGTH);
        out << std::endl;
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    std::ios::sync_with_stdio(false);
    srand((unsigned int) time(NULL));
    std::cout << CFG_TOTAL_LENGTH << std::endl;
    dna = (char*)malloc(CFG_TOTAL_LENGTH+1);
    for (unsigned long long i = 0; i < CFG_TOTAL_LENGTH; i++){
        dna[i] = types[rand()&3];
    }
    pthread_t threads[CFG_NUM_THREAD];
    for (long long i = 0; i < CFG_NUM_THREAD; i++){
        pthread_create(&threads[i], NULL, thread_main, (void*)i);
    }
    std::ostringstream ss;
    ss << CFG_FILEPATH << "total.txt";
    std::ofstream out(ss.str());
    for (unsigned long long i = 0; i < NUM_TOT_STR; i++){
        out << i << std::endl;
        unsigned long long x = ((unsigned long long) rand() << 31) + rand();
        x %= CFG_TOTAL_LENGTH-CFG_STR_LENGTH;
        out.write(&dna[x], CFG_STR_LENGTH);
        out << std::endl;
    }
    for (long long i = 0; i < CFG_NUM_THREAD; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

//
// Created by diego on 11-11-20.
//

// Pasa los tests
// Esta es la version con semaforos

#include <nSystem.h>

nSem s[3]; //Semaforos
int cnt[2] = {0, 0}; // Cuantos hay en cada espera
int ocup = -1; // Quien lo usa actualmente

void init(){
    s[0] = nMakeSem(0);
    s[1] = nMakeSem(0);
    s[2] = nMakeSem(1);
}

void pedir(int cat){
    nWaitSem(s[2]);
    if(ocup == -1){
        ocup = cat;
        nSignalSem(s[2]);
    }
    else{
        cnt[cat]++;
        nSignalSem(s[2]);
        nWaitSem(s[cat]);
    }

}

void devolver(){
    nWaitSem(s[2]);
    if (cnt[!ocup] > 0){
        cnt[!ocup]--;
        nSignalSem(s[!ocup]);
        ocup = !ocup;
    }
    else if (cnt[ocup] > 0){
        cnt[ocup]--;
        nSignalSem(s[ocup]);
    }
    else{
        ocup = -1;
    }
    nSignalSem(s[2]);
}
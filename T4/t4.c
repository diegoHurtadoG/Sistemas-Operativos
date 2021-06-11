#include <nSystem.h>
#include <fifoqueues.h>
#include "spinlock.h"
#include "pedir.h"

SpinLock q[3]; //Aqui van a ir los spinlocks
int cnt[2]; // Cuantos hay en cada espera
int ocup = -1; // Quien lo usa actualmente

void iniciar(){
    initSpinLock(&q[0], CLOSED);
    initSpinLock(&q[1], CLOSED);
    initSpinLock(&q[2], OPEN);
    cnt[0] = 0;
    cnt[1] = 0;
    //cnt[0] = MakeFifoQueue();
    //cnt[1] = MakeFifoQueue();
}

void terminar(){
    destroySpinLock(&q[0]);
    destroySpinLock(&q[1]);
    destroySpinLock(&q[2]);
    //DestroyFifoQueue(cnt[0]);
    //DestroyFifoQueue(cnt[1]);
}

void pedir(int cat){
    spinLock(&q[2]);
    if(ocup == -1){
        ocup = cat;
        spinUnlock(&q[2]);
    }
    else{
        cnt[cat]++;
        spinUnlock(&q[2]);
        spinLock(&q[cat]);
    }

}

void devolver(){
    spinLock(&q[2]);
    if (cnt[!ocup] > 0){
        cnt[!ocup]--;
        spinUnlock(&q[!ocup]);
        ocup = !ocup;
    }
    else if (cnt[ocup] > 0){
        cnt[ocup]--;
        spinUnlock(&q[ocup]);
    }
    else{
        ocup = -1;
    }
    spinUnlock(&q[2]);
}
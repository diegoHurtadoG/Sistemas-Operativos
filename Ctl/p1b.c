//
// Created by diego on 11-11-20.
//

// Pasa los tests
// Esta es la version con monitores.

#include <nSystem.h>
#include "fifoqueues.h"
typedef struct {
    FifoQueue cats0q;
    FifoQueue cats1q;
    nMonitor m;
    int ocup;
} Ctrl;

typedef struct {
    int catnum;
    nCondition cond;
    int estado;
} Request;

Ctrl c;

void init(){
    c.cats0q = MakeFifoQueue();
    c.cats1q = MakeFifoQueue();
    c.m = nMakeMonitor();
    c.ocup = -1;
}

void pedir(int cat){
    nEnter(c.m);
    Request r = {cat, nMakeCondition(c.m), 0};
    if (c.ocup == -1){
        c.ocup = cat;
        r.estado = 1;
    }
    else if(cat == 0){
        PutObj(c.cats0q, &r);
    }
    else if(cat == 1){
        PutObj(c.cats1q, &r);
    }
    while(r.estado == 0){
        nWaitCondition(r.cond);
    }
    nDestroyCondition(r.cond);
    nExit(c.m);
}

void devolver(){
    nEnter(c.m);
    if(c.ocup == 0 && (LengthFifoQueue(c.cats1q) > 0)){
        Request *p = GetObj(c.cats1q);
        p->estado = 1;
        c.ocup = p->catnum;
        nSignalCondition(p->cond);
    }
    else if(c.ocup == 1 && (LengthFifoQueue(c.cats0q) > 0)){
        Request *p = GetObj(c.cats0q);
        p->estado = 1;
        c.ocup = p->catnum;
        nSignalCondition(p->cond);
    }
    else if(c.ocup == 0 && (LengthFifoQueue(c.cats0q) > 0)){
        Request *p = GetObj(c.cats0q);
        p->estado = 1;
        c.ocup = p->catnum;
        nSignalCondition(p->cond);
    }
    else if(c.ocup == 1 && (LengthFifoQueue(c.cats1q) > 0)){
        Request *p = GetObj(c.cats1q);
        p->estado = 1;
        c.ocup = p->catnum;
        nSignalCondition(p->cond);
    }
    else{
        c.ocup = -1;
    }
    nExit(c.m);
}
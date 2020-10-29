//
// Created by diego on 25-10-20.
//
// Varon 0 y Dama 1

// Los wait Condition van en un while
// En las queue van punteros a los requests
// Si no funciona, puedo tratar cambiando el ctrl a puntero y sus referencias tambien (c-> en vez de c.)

#define FALSE 0
#define TRUE 1

#define VARON 0
#define DAMA  1

#include <nSystem.h>
#include "pub.h"
#include "fifoqueues.h"
#include <stdio.h>

typedef struct {
    FifoQueue varones;
    FifoQueue damas;
    nMonitor m;
    int varon, dama;
} Ctrl;

typedef struct {
    int sexo; // 0 si varon, 1 si dama
    nCondition cond;
    int listo; // 0 si no, 1 si si
} Request;

Ctrl c;

void ini_pub(void){
    c.m = nMakeMonitor();
    c.dama = 0;
    c.varon = 0;
    c.damas = MakeFifoQueue();
    c.varones = MakeFifoQueue();
}

void esperar(int k){
    Request r = {k, nMakeCondition(c.m), FALSE};
    if(k == VARON){
        PutObj(c.varones, &r);
    }
    else if(k == DAMA){
        PutObj(c.damas, &r);
    }
    while(r.listo == FALSE){
        nWaitCondition(r.cond);
    }
    nDestroyCondition(r.cond);
}

void despertar(int sexo){
    if(sexo == VARON){
        Request *p = GetObj(c.varones);
        if(p == NULL){
            return;
        }
        else if(c.dama == 0){ //Quiza se podria agregar condicion de queue
            p->listo = TRUE;
            nSignalCondition(p->cond);
        }
        else{
            PutObj(c.varones, p);
        }
    }
    else if(sexo == DAMA){
        Request *p = GetObj(c.damas);
        if(p == NULL){
            return;
        }
        else if(c.varon == 0){ //Quiza se podria agregar condicion de queue
            p->listo = TRUE;
            nSignalCondition(p->cond);
        }
        else{
            PutObj(c.damas, p);
        }
    }
}

void entrar(int sexo){
    nEnter(c.m);
    if(sexo == VARON){
        if(c.dama > 0 || !EmptyFifoQueue(c.damas)){ //Si hay alguna dama dentro o esperando, espera
            esperar(VARON);
        }
        c.varon++; // Cuando puede entrar, se suma a varones, si falla podria set esto por lo del foro
        despertar(VARON); // Si no funciona se puede probar poniendo dama
    }
    else if(sexo == DAMA){
        if(c.varon > 0 || !EmptyFifoQueue(c.varones)){ //Si hay algun varon dentro o esperando, espera
            esperar(DAMA);
        }
        c.dama++;
        despertar(DAMA);
    }
    nExit(c.m);
}


void salir(int sexo){
    nEnter(c.m);
    if(sexo == VARON){
        c.varon--;
        if(c.varon == 0){
            despertar(DAMA);
        }
    }
    else if(sexo == DAMA){
        c.dama--;
        if(c.dama == 0){
            despertar(VARON);
        }
    }
    nExit(c.m);
}



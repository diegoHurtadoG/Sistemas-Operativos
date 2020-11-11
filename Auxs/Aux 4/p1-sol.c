#include <stdio.h>
#include "nSystem.h"

#define BUSCANDO 1
#define LISTO 2
#define FIN 3

typedef struct Nodo {
    struct Nodo *izq, *der;
    int valor;
} Nodo;

int valorA, valorB;
int estadoA, estadoB;

nMonitor ctrl;

Nodo *a, *b;

Nodo *crear(int valor, Nodo *izq, Nodo *der) {
    Nodo *p = (Nodo *) nMalloc(sizeof(Nodo));
    p->valor = valor;
    p->izq = izq;
    p->der = der;
    return p;
}

int recorrerArbol(Nodo *raiz, int *estado, int *valor);
int recorrerRec(Nodo *nodo, int *estado, int *valor);

int nMain(){
    a = crear(10,
            crear(7, crear(3, NULL ,NULL), crear(9, NULL ,NULL)),
            crear(13, crear (12, NULL ,NULL), NULL));
    b = crear(8,
        crear(3, NULL ,NULL),
        crear(15, crear (12, NULL ,NULL),NULL));
    ctrl = nMakeMonitor ();
    estadoA = BUSCANDO;
    estadoB = BUSCANDO;
    nTask t1=nEmitTask(recorrerArbol, a, &estadoA, &valorA);
    nTask t2=nEmitTask(recorrerArbol, b, &estadoB, &valorB);
    nPrintf("\nLista ordenada de nodos:");
    nEnter(ctrl);
    for(;;) {
        while(estadoA == BUSCANDO || estadoB == BUSCANDO)
            nWait(ctrl);
        int sgte;
        if(estadoA == FIN && estadoB == FIN)
            break;
        else if(estadoA == FIN) {
            sgte = valorB;
            estadoB = BUSCANDO;
        } else if(estadoB == FIN){
            sgte = valorA;
            estadoA = BUSCANDO;
        } else if(valorA <= valorB){
            sgte = valorA;
            estadoA = BUSCANDO;
        } else {
            sgte = valorB;
            estadoB = BUSCANDO;
        }
        nPrintf(" %i", sgte);
        nNotifyAll(ctrl);
    }
    nExit(ctrl);
    nWaitTask(t1);
    nWaitTask(t2);
    nPrintf("\n\n");
    return 0;
}

int recorrerArbol(Nodo *raiz, int *estado, int *valor){ 
    recorrerRec(raiz, estado, valor);
    nEnter(ctrl);
    while(* estado != BUSCANDO)
        nWait(ctrl);
    *estado = FIN;
    nNotifyAll(ctrl);
    nExit(ctrl);
    return 0;
}

int recorrerRec(Nodo *nodo, int *estado, int *valor) {
    if(nodo == NULL)
        return 0;
    recorrerRec(nodo ->izq, estado, valor);
    nEnter(ctrl);
    while(* estado != BUSCANDO)
        nWait(ctrl);
    *valor = nodo ->valor;
    *estado = LISTO;
    nNotifyAll(ctrl);
    nExit(ctrl);
    recorrerRec(nodo ->der, estado, valor);
    return 0;
}

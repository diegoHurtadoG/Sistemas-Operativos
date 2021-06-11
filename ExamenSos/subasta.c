#include <nSystem.h>
#include <stdio.h>
#include "subasta.h"

// Paso los tests :)

struct subasta{
    PriQueue c;
    int n;
    SpinLock sl;
};

typedef struct {
    int oferta;
    int resol;  // 1 -> aprobada
                // 0 -> rechazada
                // -1 -> pendiente
    SpinLock ofsl;
} Req;

// Agregue la estructura Req

Subasta nuevaSubasta(int n) {
    Subasta s = malloc(sizeof(*s));
    s->n = n;
    s->c = MakePriQueue(n+1);
    initSpinLock(&(s->sl), OPEN);
    return s;
}

int ofrecer(Subasta s, int oferta) {
    spinLock(&(s->sl));
    Req r = {oferta, -1};
    initSpinLock(&(r.ofsl), CLOSED);
    PriPut(s->c, &r, oferta);
    if (PriSize(s->c)>s->n){
        Req *pr = PriGet(s->c);
        pr->resol = 0; //Se rechaza
        spinUnlock(&(pr->ofsl));
    }
    spinUnlock(&(s->sl));
    spinLock(&(r.ofsl)); // Aqui se espera
    return r.resol;
}

int adjudicar(Subasta s, int *punid) {
    spinLock(&(s->sl));
    int recaud = 0;
    *punid = 0;
    while (!EmptyPriQueue(s->c)){
        Req *pr = PriGet(s->c);
        pr->resol = 1;
        (*punid)++;
        recaud += pr->oferta;
        spinUnlock(&(pr->ofsl));//***
    }
    s->n -= *punid;
    spinUnlock(&(s->sl));
    return recaud;
}

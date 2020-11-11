#include "nSystem.h"
#include "nSysimp.h"
#include "fifoqueues.h"

//... declare variables globales ...
// Funcion util LengthFifoQueue
// PutObj(cola, task), Tambien puede ser PushObj
// GetObj(cola)
FifoQueue pending_oxy;
FifoQueue pending_hydro;

void H2oInit() {
  //... función de inicializacion ...
  //... invoque esta funcion desde nMain.c ...
  pending_oxy = MakeFifoQueue();
  pending_hydro = MakeFifoQueue();
}

// Esta funcion es invocada desde testh2o.c
nH2o nCombineOxy(void *oxy, int timeout) {
    START_CRITICAL();
    nTask this_task = current_task;

    this_task->atomo = oxy;
    PutObj(pending_oxy, this_task);

    if(LengthFifoQueue(pending_hydro) >= 2 && LengthFifoQueue(pending_oxy) >= 1){
        nH2o mol = (nH2o) nMalloc(sizeof(*mol));

        /* SACAMOS LA PRIMERA DE HYDRO Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pHydro1 = GetObj(pending_hydro);
        mol->hydro1 = pHydro1->atomo;
        pHydro1->molecula = mol;
        pHydro1->status = READY;

        /* SACAMOS LA SEGUNDA DE HYDRO Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pHydro2 = GetObj(pending_hydro);
        mol->hydro2 = pHydro2->atomo;
        pHydro2->molecula = mol;
        pHydro2->status = READY;

        /* SACAMOS LA DE OXY Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pOxy = GetObj(pending_oxy);
        mol->oxy = pOxy->atomo;
        pOxy->molecula = mol;
        pOxy->status = READY;

        /* METEMOS LAS TRES A LA READY */
        PushTask(ready_queue, pHydro1);
        PushTask(ready_queue, pHydro2);
        PushTask(ready_queue, pOxy);
    }
    else if(LengthFifoQueue(pending_hydro) >= 2){ // Hay hydro pero no oxi
        current_task->status = WAIT_O;
    }
    else if(LengthFifoQueue(pending_oxy) >= 1){ // Hay oxi pero no hydro
        current_task->status = WAIT_H;
    }
    else{ // No hay niuna
        current_task->status = WAIT_H; // Si llego a algun punto ciego puedo separar esta espera a una aparte
    }
    ResumeNextReadyTask();
    //struct nH2o rm = this_task->molecula;

    END_CRITICAL();
    return this_task->molecula;
}

// Esta funcion es invocada desde testh2o.c
nH2o nCombineHydro(void *hydro) {
    START_CRITICAL();
    nTask this_task = current_task;

    this_task->atomo = hydro;
    PutObj(pending_hydro, this_task);
    if(LengthFifoQueue(pending_hydro) >= 2 && LengthFifoQueue(pending_oxy) >= 1){
        nH2o mol = (nH2o) nMalloc(sizeof(*mol));

        /* SACAMOS LA PRIMERA DE HYDRO Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pHydro1 = GetObj(pending_hydro);
        mol->hydro1 = pHydro1->atomo;
        pHydro1->molecula = mol;
        pHydro1->status = READY;

        /* SACAMOS LA SEGUNDA DE HYDRO Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pHydro2 = GetObj(pending_hydro);
        mol->hydro2 = pHydro2->atomo;
        pHydro2->molecula = mol;
        pHydro2->status = READY;

        /* SACAMOS LA DE OXY Y ASIGNAMOS CAMPOS A MOL Y A LA TAREA */
        nTask pOxy = GetObj(pending_oxy);
        mol->oxy = pOxy->atomo;
        pOxy->molecula = mol;
        pOxy->status = READY;

        /* METEMOS LAS TRES A LA READY */
        PushTask(ready_queue, pHydro1);
        PushTask(ready_queue, pHydro2);
        PushTask(ready_queue, pOxy);
    }
    else if(LengthFifoQueue(pending_hydro) >= 2){ // Hay hydro pero no oxi
        current_task->status = WAIT_O;
    }
    else if(LengthFifoQueue(pending_oxy) >= 1){ // Hay oxi pero no hydro
        current_task->status = WAIT_H;
    }
    else{ // No hay niuna
        current_task->status = WAIT_H; // Si llego a algun punto ciego puedo separar esta espera a una aparte
    }
    ResumeNextReadyTask();
    //struct nH2o rm = this_task->molecula;

    END_CRITICAL();
    return this_task->molecula;
}

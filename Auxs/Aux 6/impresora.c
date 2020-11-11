/*** implementacion ***/
#define OBTENER 1
#define DEVOLVER 2
#define OK 3

// Esta es la tarea que administra la impresora
nTask impresora;

void obtenerImpresora() {
    // Enviar el mensaje de "Obtener" la impresora
    int msg = OBTENER;
    nSend(impresora , &msg);
}

void devolverImpresora() {
    // Enviar el mensaje de "Devolver" la impresora
    int msg = DEVOLVER;
    nSend(impresora , &msg);
}

int impresoraServer() {
    nSetTaskName("TASK impresoraServer");
    nTask t;
    int *msg;
    // Cola de trabajos que deben esperar
    FifoQueue printerQueue = MakeFifoQueue();
    int busy = FALSE;
    for(;;) {
        if(busy == FALSE) {
            // Recibir mensajes por cierto tiempo
            msg = (int*) nReceive(&t, MILISEGUNDOS_DORMIR);
            if(t == NULL) {
                // Se cumplio el tiempo sin recibir mensajes , pasar a bajo consumo
                modoBajoConsumo ();
                // Esperar indefinidamente por un trabajo
                msg = (int*) nReceive(&t, -1);
                // Despertar la impresora cuando estaba dormida
                modoUsoNormal ();
            }
        } else {
            // Si la impresora esta ocupada, esperar indefinidamente
            // por el mensaje "Devolver"
            msg = (int*) nReceive (&t, -1);
        }
        
        if(*msg == OBTENER) {
            if(busy == TRUE) {
                // La impresora esta ocupada, agrego la solicitud a la fila
                PutObj(printerQueue, t);
            } else {
                // Ocupar la impresora
                busy = TRUE;
                nReply(t, OK);
            }
        } else if(*msg == DEVOLVER) {
            // Responder a la tarea
            nReply(t, OK);
            // Ver si hay otra tarea esperando por la impresora
            if(!EmptyFifoQueue(printerQueue)) {
                nTask *t2 = (nTask*) GetObj(printerQueue);
                nReply(t2, OK);
            } else {
                // Si no hay otra tarea , la impresora queda disponible
                busy = FALSE;
            }
        }
    }
}

void inicializarImpresora() {
    // Este procedimiento solo lanza la tarea con el printServer
    impresora = nEmitTask(impresoraServer);
}

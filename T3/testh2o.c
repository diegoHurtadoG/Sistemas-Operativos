#include <nSystem.h>

/* Ver el nMain al final */

/****************************************************
 * Tareas para la prueba y verificacion
 ****************************************************/

static int oxygen(nH2o *ph2o, int timeout) {
  *ph2o= nCombineOxy(ph2o, timeout);
  return 0;
}

static int hydrogen(nH2o *ph2o) {
  *ph2o= nCombineHydro(ph2o);
  return 0;
}

static int oxy_bis(nH2o *ph2o) {
  *ph2o= nCombineOxy(ph2o, -1);
  return 0;
}

static int hydro_bis(nTask *ptask, nH2o *ph2o) {
  *ph2o= nCombineHydro(ptask);
  return 0;
}

/****************************************************
 * Programa principal
 ****************************************************/

#define N_TASKS 10000
#define N_ITER 1000

nTask ot[N_TASKS], ht[N_TASKS*2];
nH2o o_h2o[N_TASKS], h_h2o[N_TASKS*2];

int nMain(int argc, char **argv) {
  /* Test de prueba: semantica sin timeouts */
  {
    nTask H1, H2, H3, H4, O1, O2;
    nH2o H1_h2o, H2_h2o, H3_h2o, H4_h2o, O1_h2o, O2_h2o;
    nPrintf("Lanzando hidrogeno 1\n");
    H1= nEmitTask(hydrogen, &H1_h2o);
    nSleep(300);
    nPrintf("Lanzando oxigeno 1\n");
    O1= nEmitTask(oxygen, &O1_h2o, -1);
    nSleep(300);
    nPrintf("Lanzando oxigeno 2\n");
    O2= nEmitTask(oxygen, &O2_h2o, -1);
    nSleep(300);
    nPrintf("Lanzando hidrogeno 2\n");
    H2= nEmitTask(hydrogen, &H2_h2o);
    nWaitTask(H1); nWaitTask(O1); nWaitTask(H2);
    nH2o h2o= O1_h2o;
    if ( h2o!=H1_h2o || h2o!=H2_h2o)
      nFatalError("nMain", "No es la misma molecula para H1, H2 y O1\n");
    if ( h2o->oxy != &O1_h2o )
      nFatalError("nMain", "Atomo de oxigeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H1_h2o || h2o->hydro2==&H1_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H2_h2o || h2o->hydro2==&H2_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    nSleep(300);
    nPrintf("Lanzando hidrogeno 3\n");
    H3= nEmitTask(hydrogen, &H3_h2o);
    nSleep(300);
    nPrintf("Lanzando hidrogeno 4\n");
    H4= nEmitTask(hydrogen, &H4_h2o);
    nWaitTask(O2); nWaitTask(H3); nWaitTask(H4);
    h2o= O2_h2o;
    if ( h2o!=H3_h2o || h2o!=H4_h2o)
      nFatalError("nMain", "No es la misma molecula para H3, H4 y O2\n");
    if ( h2o->oxy != &O2_h2o )
      nFatalError("nMain", "Atomo de oxigeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H3_h2o || h2o->hydro2==&H3_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H4_h2o || h2o->hydro2==&H4_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    nPrintf("test de la semantica sin timeout: ok\n");
  }

#ifdef TIMEOUTS
  /* Test de prueba: semantica con timeouts */
  {
    nTask H1, H2, O1, O2, O3;
    nH2o H1_h2o, H2_h2o, O1_h2o, O2_h2o, O3_h2o;
    O1= nEmitTask(oxygen, &O1_h2o, 300);
    nWaitTask(O1);
    if (O1_h2o!=NULL)
      nFatalError("nMain", "O1 no debio haber formado una molecula\n");
    H1= nEmitTask(hydrogen, &H1_h2o);
    nSleep(300);
    O2= nEmitTask(oxygen, &O2_h2o, 300);
    nSleep(150);
    nWaitTask(O2);
    if (O2_h2o!=NULL)
      nFatalError("nMain", "O2 no debio haber formado una molecula\n");
    O3= nEmitTask(oxygen, &O3_h2o, -1);
    H2= nEmitTask(hydrogen, &H2_h2o);
    nWaitTask(H1); nWaitTask(O3); nWaitTask(H2);
    nH2o h2o= H1_h2o;
    if ( h2o!=O3_h2o || h2o!=H2_h2o)
      nFatalError("nMain", "No es la misma molecula para H1, O3 y H2\n");
    if ( ! (h2o->hydro1==&H1_h2o || h2o->hydro2==&H1_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H2_h2o || h2o->hydro2==&H2_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    nPrintf("test de la semantica con timeout: ok\n");
  }

  /* Test de prueba: variacion para la semantica con timeouts */
  {
    nTask H1, H2, O1, O2, O3;
    nH2o H1_h2o, H2_h2o, O1_h2o, O2_h2o, O3_h2o;
    O1= nEmitTask(oxygen, &O1_h2o, 300);
    nWaitTask(O1);
    if (O1_h2o!=NULL)
      nFatalError("nMain", "O1 no debio haber formado una molecula\n");
    O2= nEmitTask(oxygen, &O2_h2o, 300);
    nSleep(150);
    H1= nEmitTask(hydrogen, &H1_h2o);
    nWaitTask(O2);
    if (O2_h2o!=NULL)
      nFatalError("nMain", "O2 no debio haber formado una molecula\n");
    O3= nEmitTask(oxygen, &O3_h2o, -1);
    nSleep(100);
    H2= nEmitTask(hydrogen, &H2_h2o);
    nWaitTask(H1); nWaitTask(O3); nWaitTask(H2);
    nH2o h2o= H1_h2o;
    if ( h2o!=O3_h2o || h2o!=H2_h2o)
      nFatalError("nMain", "No es la misma molecula para H1, O3 y H2\n");
    if ( ! (h2o->hydro1==&H1_h2o || h2o->hydro2==&H1_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H2_h2o || h2o->hydro2==&H2_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    nPrintf("variacion del test de la semantica con timeout: ok\n");
  }

  /* Test de prueba: otra variacion para la semantica con timeouts */
  {
    nTask H1, H2, O2, O3;
    nH2o H1_h2o, H2_h2o, O2_h2o, O3_h2o;
    O3= nEmitTask(oxygen, &O3_h2o, 2000);
    nSleep(200);
    O2= nEmitTask(oxygen, &O2_h2o, 400);
    nSleep(200);
    H1= nEmitTask(hydrogen, &H1_h2o);
    nSleep(200);
    nWaitTask(O2);
    if (O2_h2o!=NULL)
      nFatalError("nMain", "O2 no debio haber formado una molecula\n");
    H2= nEmitTask(hydrogen, &H2_h2o);
    nWaitTask(H1); nWaitTask(O3); nWaitTask(H2);
    nH2o h2o= H1_h2o;
    if ( h2o!=O3_h2o || h2o!=H2_h2o)
      nFatalError("nMain", "No es la misma molecula para H1, O3 y H2\n");
    if ( ! (h2o->hydro1==&H1_h2o || h2o->hydro2==&H1_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    if ( ! (h2o->hydro1==&H2_h2o || h2o->hydro2==&H2_h2o) )
      nFatalError("nMain", "Atomo de hidrogeno es incorrecto\n");
    nPrintf("otra variacion del test de la semantica con timeout: ok\n");
  }

#endif

  /* Test de prueba: robustez */
  {
    nPrintf("Iniciando test de robustez.  Apareceran 1000 puntos.\n");
    nSetTimeSlice(1);
    for (int i= 0; i<N_TASKS; i++) {
      ot[i] = nEmitTask(oxy_bis, &o_h2o[i]);
      ht[2*i]= nEmitTask(hydro_bis, &ht[2*i], &h_h2o[2*i]);
      ht[2*i+1]= nEmitTask(hydro_bis, &ht[2*i+1], &h_h2o[2*i+1]);
    }
    for (int k=1; k<N_ITER; k++) {
      for (int i= 0; i<N_TASKS; i++) {
        nWaitTask(ot[i]);
        nH2o h2o= o_h2o[i];
        nTask *pth1= h2o->hydro1;
        nTask *pth2= h2o->hydro2;
        int i1= pth1-ht;
        int i2= pth2-ht;
        // nPrintf("k=%d i=%d i1=%d i2=%d\n", k, i, i1, i2);
        if (i1<0 || i1>=N_TASKS*2)
          nFatalError("nMain", "Indice fuera de rango\n");
        if (i2<0 || i2>=N_TASKS*2)
          nFatalError("nMain", "Indice fuera de rango\n");
        nWaitTask(*pth1);
        nWaitTask(*pth2);
        if (h_h2o[i1]!=h2o)
          nFatalError("nMain", "Molecula inconsistente\n");
        if (h_h2o[i2]!=h2o)
          nFatalError("nMain", "Molecula inconsistente\n");
        nFree(h2o);
        ot[i]= nEmitTask(oxygen, &o_h2o[i], -1);
        *pth1= nEmitTask(hydro_bis, pth1, &h_h2o[i1]);
        *pth2= nEmitTask(hydro_bis, pth2, &h_h2o[i2]);
      }
      nPrintf(".");
    }
    for (int i= 0; i<N_TASKS; i++) {
      nWaitTask(ot[i]);
      nH2o h2o= o_h2o[i];
      nTask *pth1= h2o->hydro1;
      nTask *pth2= h2o->hydro2;
      nWaitTask(*pth1);
      nWaitTask(*pth2);
      int i1= pth1-ht;
      if (i1<0 || i1>=N_TASKS*2)
        nFatalError("nMain", "Indice fuera de rango\n");
      if (h_h2o[i1]!=h2o)
        nFatalError("nMain", "Molecula inconsistente\n");
      int i2= pth2-ht;
      if (i2<0 || i2>=N_TASKS*2)
        nFatalError("nMain", "Indice fuera de rango\n");
      if (h_h2o[i2]!=h2o)
        nFatalError("nMain", "Molecula inconsistente\n");
      nFree(h2o);
    }
    nPrintf("Su tarea satisface test de robustez\n");
    nPrintf("Deben haber mas de 1000 cambios de contexto implicitos\n");
  }

  return 0;
}


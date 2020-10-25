#include <nSystem.h>

#include "equipo.h"

char **equipo; // = nMalloc(5*sizeof(char*)); equipo en formación
int k= 0;      // cuantos jugadores han llegado
nSem m;        // = nMakeSem(1); para garantizar la exclusión mutua
nSem **jugadores;  // Semaforo de cada jugador

void init_equipo(void) {
  equipo= nMalloc(5*sizeof(char*));
  m= nMakeSem(1);
  jugadores= nMalloc(4*sizeof(nSem*));
}

// Caso raro en k = 6, si llega ese k antes de que el el k se vuelva a poner en 0, cosa de que entra un
//  k al else y crea 4 tickets para "tickets" sin haber jugadores esperando

// Caso raro es que mientras el cuarto este haciendo el signalsem, el quinto pase al else
char **hay_equipo(char *nom) {
  nWaitSem(m); // inicio secc. crítica, pido el ticker de m
  equipo[k++]= nom; // Meto el nombre al equipo
  char **miequipo= equipo; //A la variable equipo la restringo para que sea 1 equipo
  if (k!=5) { // aún no hay equipo
    nSem sem = nMakeSem(0); //Creo el semaforo de cada jugador
    jugadores[k-1] = &sem; // Lo meto al arreglo
    nSignalSem(m); // fin secc. crít. Devuelvo el ticket a m
    nWaitSem(sem); // Espero a cada jugador
    nDestroySem(sem);// Destruye el semaforo del jugador
  }
  else { // hay equipo: despertar a los 4 jugadores en espera
    for (int i= 0; i<4; i++)
      nSignalSem(*jugadores[i]); //Meto 4 tickets al semaforo tickets
    // preparar nuevo equipo
    equipo= nMalloc(5*sizeof(char*));//Dejo listo el malloc para el proximo
    jugadores = nMalloc(4*sizeof(nSem*));
    k= 0; // Dejo listo el k para el siguiente
    nSignalSem(m); // fin secc. crít. y deposito 1 ticket en m
  }
  return miequipo;
}

#include "nSystem.h"
#include "nSysimp.h"

int ocup;
Queue cats1, cats0;

void pedirInit() {
  ocup = -1;
  cats0 = MakeQueue();
  cats1 = MakeQueue();
}

void nPedir(int cat) {
  START_CRITICAL();
  nTask tarea = current_task;
  tarea->num = cat;

  if(ocup == -1){
      tarea->status = READY;
      ocup = cat;
      PushTask(ready_queue, tarea);
  }
  else{
      tarea->status = WAIT_PEDIR;
      if(cat == 0){
          PutTask(cats0, tarea);
      }
      else{
          PutTask(cats1, tarea);
      }
  }
  ResumeNextReadyTask();
  END_CRITICAL();
}

void nDevolver() {
    START_CRITICAL();
    if(ocup == 0 && !(EmptyQueue(cats1))){
        nTask act = GetTask(cats1);
        act->status = READY;
        ocup = act->num;
        ResumeNextReadyTask();
    } else if(ocup == 1 && !(EmptyQueue(cats0))){
        nTask act = GetTask(cats0);
        act->status = READY;
        ocup = act->num;
        ResumeNextReadyTask();
    } else if(ocup == 0 && !(EmptyQueue(cats0))){
        nTask act = GetTask(cats0);
        act->status = READY;
        ocup = act->num;
        ResumeNextReadyTask();
    } else if(ocup == 1 && !(EmptyQueue(cats1))){
        nTask act = GetTask(cats1);
        act->status = READY;
        ocup = act->num;
        ResumeNextReadyTask();
    } else{
        ocup = -1;
    }
    END_CRITICAL();
}

# No olvide configurar la variable de ambiente NSYSTEM
# con la ubicación de nSystem
# NSYSTEM= ...
#
# Nota: imposible correr valgrind con nSystem

LIBNSYS=$(NSYSTEM)/lib/libnSys.a

CFLAGS= -g -Wall -pedantic -std=c99 -I$(NSYSTEM)/include
LDLIBS= $(LIBNSYS)

all: testh2o-a testh2o-b 

testh2o-a: testh2o.c $(LIBNSYS)
	gcc $(CFLAGS) testh2o.c -o testh2o-a $(LDLIBS)

testh2o-b: testh2o.c $(LIBNSYS)
	gcc $(CFLAGS) -DTIMEOUTS testh2o.c -o testh2o-b $(LDLIBS)

clean:
	rm -f *.o *~ testh2o-a testh2o-b

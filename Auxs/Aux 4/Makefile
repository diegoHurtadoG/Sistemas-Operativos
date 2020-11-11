# Observaciones importantes:
#
# - Para usar este Makefile es necesario definir la variable
#   de ambiente NSYSTEM con el directorio en donde se encuentra
#   la raiz de nSystem.  Por ejemplo en sh esto se hace con:
#
#   $ NSYSTEM=$HOME/nsystem64 ; export NSYSTEM
#
# - Compile ingresando el comando make
#
# - Depure usando gdb o ddd.

CFLAGS= -ggdb -std=c99 -Wall -pedantic -I$(NSYSTEM)/include
LFLAGS= -ggdb

all: ejemplo

ejemplo: p1-sol.c
	gcc $(CFLAGS) p1-sol.c $(NSYSTEM)/lib/libnSys.a -o p1-sol

clean:
	rm -f *.o *~

cleanall:
	rm -f *.o *~ p1-sol.c

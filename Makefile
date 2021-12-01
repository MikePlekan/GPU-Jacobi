CC=gcc
CFILES=timer.c jacobi.c
OFILES=$(CFILES:.c=.o)

jacobi:	$(OFILES)
	$(CC) -o jacobi $(OFILES) -lm -fopenmp -L/usr/local/cuda/lib64 -lcudart

jacobi.o:	timer.h

timer.o:	timer.h

.c.o:
	$(CC) -c $<

clean::
	/bin/rm $(OFILES) jacobi
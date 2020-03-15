DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFIL := files/

CFLAGS := -I $(DIRHEA)
LDLIBS := -lm
CC := mpicc
RUN := mpirun

all : dirs hipercubeNet

dirs:
	mkdir -p $(DIREXE)

hipercubeNet:
	$(CC) $(DIRSRC)hipercubeNet.c $(CFLAGS) $(LDLIBS) -o $(DIREXE)hipercubeNet

solution:
	$(RUN) -n 8 ./$(DIREXE)hipercubeNet 

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 

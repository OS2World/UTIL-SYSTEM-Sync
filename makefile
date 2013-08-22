#makefile for sync, emx09a and dmake

default: sync.exe
CFLAGS=-O2 -s -Zomf -Zsys
LDFLAGS=-Zomf -s -Zsys -O/NOS -O/A:4 -O/BAS:0x10000 -O/EXEPACK:2 -O/PACKDATA -O/PACKCODE

sync.exe: sync.o makefile
	$(LD) $(LDFLAGS) -o sync.exe sync.o sync.def
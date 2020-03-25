assembler: assembler.o base.o file.o globals.o errors.o parse.o
	gcc -g -Wall -ansi -pedantic assembler.o base.o file.o globals.o errors.o parse.o -o assembler

assembler.o: assembler.c assembler.h errors.h base.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

parse.o: parse.c globals.h errors.h
	gcc -c -Wall -ansi -pedantic parse.c -o parse.o

base.o: base.c base.h globals.h file.h errors.h parse.h
	gcc -c -Wall -ansi -pedantic base.c -o base.o

globals.o: globals.c globals.h
	gcc -c -Wall -ansi -pedantic globals.c -o globals.o

file.o: file.c file.h globals.h errors.h
	gcc -c -Wall -ansi -pedantic file.c -o file.o

parse.o: parse.c parse.h globals.h errors.h
	gcc -c -Wall -ansi -pedantic parse.c -o parse.o
	
errors.o: errors.c errors.h globals.h
	gcc -c -Wall -ansi -pedantic errors.c -o errors.o
assembler: assembler.o analyze.o first_pass.o second_pass.o
	gcc -g assembler.o analyze.o first_pass.o second_pass.o -ansi -Wall -pedantic -o assembler
assembler.o: assembler.c assembler.h 
	gcc -c assembler.c -ansi -Wall -pedantic -o assembler.o
analyze.o: analyze.c analyze.h
	gcc -c analyze.c -ansi -Wall -pedantic -o analyze.o
first_pass.o: first_pass.c analyze.h
	gcc -c first_pass.c -ansi -Wall -pedantic -o first_pass.o
second_pass.o: second_pass.c analyze.h
	gcc -c second_pass.c -ansi -Wall -pedantic -o second_pass.o

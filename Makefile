all : compilar

compilar : sintatico.c sintatico.h lexico.o
	gcc -g sintatico.c -g lexico.c -o compilar -lm -Wall

lexico.o : lexico.c lexico.h
	gcc lexico.c -c -lm -Wall

.PHONY:clean

clean:
	rm -f *.o
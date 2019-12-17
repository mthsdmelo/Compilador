#ifndef __DEF__
#define __DEF__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_PALAVRA 32
#define MAX_NUMERO 32
#define MAX_LITERAL 200

#define ID 0
#define NUM 1
#define LITERAL 2

typedef struct t{
    short id;
    char palavra[MAX_PALAVRA];
    char numero[MAX_NUMERO];
    char literal[MAX_LITERAL];
    int coluna;
    int linha;
} Token;


#define ADD 25            // +
#define SUB 26            // -
#define MUL 27            // *
#define DIV 28            // /
#define IGUAL 29          // =
#define OR 30             // ||
#define AND 31            // &&
#define ADD_ADD 32        // ++
#define SUB_SUB 33        // --
#define ADD_IGUAL 34      // +=
#define MUL_IGUAL 35      // *=
#define DIV_IGUAL 36      // /=
#define SUB_IGUAL 37      // -=
#define IGUAL_IGUAL 38    // ==
#define MAIOR 39          // >
#define MENOR 40          // <
#define MAIOR_IGUAL 41    // >=
#define MENOR_IGUAL 42    // <=
#define DIFERENTE 43      // !=


#define PONTO_VIRGULA 44  // ;
#define O_COLCHETE 45     // [
#define C_COLCHETE 46     // ]
#define VIRGULA 47        // ,
#define O_PARENTESES 48   // (
#define C_PARENTESES 49   // )
#define ASPAS_D 50        // "
#define ASPAS_S 51        // '
#define O_CHAVE 52        // {
#define C_CHAVE 53        // }
#define DOIS_PONTOS 54    // :

#define FUNCAO 55
#define FIM 56


#endif // __DEF__

#ifndef __LEX
#define __LEX

#include "definicoes.h"

void gera_token_palavra(char palavra[], int coluna, int linha, FILE * arq);

void gera_token_separador(char c, int coluna, int linha, FILE * arq) ;

void gera_token_numero(char numero[], int coluna, int linha, FILE * arq) ;

void gera_token_operador(char c[], int coluna, int linha, FILE * arq) ;

void gera_token_literal(char str[], FILE * arq) ;

void gera_tokens(char * filename) ;

#endif // __LEX

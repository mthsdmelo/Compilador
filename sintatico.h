#ifndef __SINT__
#define __SINT__

#include "lexico.h"

typedef struct var{
    char nome[MAX_PALAVRA];
    int tipo;
    int nreg;
}Variaveis;

struct esc{
    char escopo[MAX_PALAVRA];
    struct esc *prox;
};

typedef struct pilha_escopo{
    struct esc *topo;
}Pilha_escopo;

typedef struct lbreak{
    int n;
    struct lbreak *p;
}Lbreak;

typedef struct pilha_l{
    struct lbreak *topo;
}Pilha_l;

int lista_declaracao();
int lista_ids();
int bloco_for();
int expressao();
int chamada_funcao();
int erro(const char *e);
int elemento();
int operadores();
int expressao_aritimetica();
int operadores_booleanos();
int operadores_comparacao();
int operadores_duplos();
int operadores_composto();
int expressao2();
void ler_tokens();
void last_token();
void next_token();
void reset_token(int i);
int tipo();
int identificador();
int lista_declaracao();
int lista_ids_com_expressao();
int declaracao();
int bloco_else();
int bloco_if();
int bloco_escopo();
int bloco_while();
int bloco_for();
int bloco();
int funcao();

#endif //__SINT__

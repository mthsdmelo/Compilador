#include "sintatico.h"
#include "lexico.h"


Token *tokens;
Token token;
Token anterior;
FILE *arq;
FILE *arq_erro;
int n_erros;
int debug = 0;
int INDICE = 0;
int Tamanho;
int numero_loops=1;
int numero_ifs = 1;
int numero_registradores=1;
Variaveis *v;
int nvars = 0;
Pilha_escopo pilha_esc;
Pilha_l p;

void criar_lista_var(char nome[],int tipo){
    v = malloc(sizeof(Variaveis)*100);
    strcpy(v[nvars].nome,nome);
    v[nvars].tipo = tipo;
    nvars++;
}

void adicionar_lista_var(char nome[],int tipo,int n){
    strcpy(v[nvars].nome,nome);
    v[nvars].tipo = tipo;
    v[nvars].nreg = n;
    nvars++;
}

int existe_em_lista_var(char *nome){
    for(int i=0;i<nvars;i++){
        if(strcmp(v[i].nome,nome)==0){
            return v[i].nreg;
        }
    }
    return 0;
}

int chamada_funcao(){
    int indice = INDICE;
    if(identificador()){
        next_token();
        if(token.id==O_PARENTESES){
            if(lista_parametro_cahamada_funcao()){
                next_token();
                if(token.id==C_PARENTESES){
                    return 1;
                }
                else{
                    return erro ("parenteses chamada de funcao");
                }
            }
        } 
    }
    reset_token(indice);
    return 0;
}

void empilhar_escopo(char esc[]){
    struct esc e;
    strcpy(e.escopo, esc);
    e.prox = pilha_esc.topo;
    pilha_esc.topo = &e;
}

char *desempilhar_escopo(){
    char *s = malloc(sizeof(char)*MAX_PALAVRA);
    if(pilha_esc.topo->prox!=NULL){
        strcpy(s, pilha_esc.topo->escopo);
        pilha_esc.topo = pilha_esc.topo->prox;
        return s;
    }
}

void empilhar_l(int a){
    numero_loops++;
    Lbreak *x = malloc(sizeof(Lbreak));
    x->n = a;
    x->p = p.topo;
    p.topo = x;
}

int desempilhar_l(){
    if(p.topo!=NULL){
        int r = p.topo->n;
        p.topo = p.topo->p;
        return r;
    }
    return 0;
}
char *topo_escopo(){
    char *s = malloc(sizeof(char)*MAX_PALAVRA);
    strcpy(s, pilha_esc.topo->escopo);
    return s;
}


int erro(const char *e){
  printf("Erro de %s Linha: %d, Coluna %d, ID: %d\n",e,token.linha,token.coluna,token.id);
  n_erros++;
  return 0;
}

int elemento(){
    int indice = INDICE;
    if(chamada_funcao()){
        return 1;
    }
    next_token();
    if(token.id==ID){
        if((existe_em_lista_var(token.palavra))){
            return 1;
        }
        else
            return erro("variavel nao declarada");
    }
    if(token.id==NUM){
        //printf("li $t%d, %s\n",numero_registradores,token.numero);
       // numero_registradores++;
        return 1;
    }
    if(token.id==LITERAL){
       // printf("li $t%d, %s\n",numero_registradores,token.literal);
       // numero_registradores++;
        return 1;
    } 

    reset_token(indice);
    return 0;
}
int operadores(){
    int indice = INDICE;
    next_token();
    if(token.id==ADD||token.id==SUB||token.id==MUL||token.id==DIV){
        return token.id;
    }
    reset_token(indice);
    return 0;
}


int expressao_aritimetica(){
    int indice = INDICE;
    int tamanho = 0;
    while (1){ 
        if(elemento()){
            tamanho++;
            if(operadores()){
                tamanho++;
                continue;
            }
            else{
                break;
            }
        }
        next_token();
        if(token.id==O_PARENTESES){
            tamanho++;
            next_token();
            while (token.id!=C_PARENTESES){
                last_token();
                if(elemento()){
                    tamanho++;
                    if(operadores()){
                        tamanho++;
                        next_token();
                        continue;
                    }
                    next_token();
                    if(token.id==C_PARENTESES){
                        tamanho++;
                        break;
                    }
                    return erro("expressao aritmetica");
                }
                return erro("expressao aritmetica");
            }
            if(operadores()){
                tamanho++;
                continue;
            }
            return erro("Expressao aritimetica");
        }
        if(tamanho){
            break;
        }
        reset_token(indice);
        return 0;

    }
    for(int i = indice;i<indice+tamanho;i++){
        int a , b;
        if(tokens[i].id==ID){
            b = a; 
            a = existe_em_lista_var(tokens[i].palavra);
        }
        if(tokens[i].id==NUM){
            b = a; 
            a = numero_registradores;
            printf("li $t%d, %s\n",numero_registradores,token.numero);
            numero_registradores++;
        }
        if(tokens[i].id==LITERAL){
            b = a; 
            a = numero_registradores;
            printf("li $t%d, %s\n",numero_registradores,token.literal);
            numero_registradores++;
        }
        if(tokens[i].id==ADD){
            printf("add $t%d, $t%d, $t%d\n",numero_registradores,a,b);
            numero_registradores++;
        }
        if(tokens[i].id==SUB){
            printf("sub $t%d, $t%d, $t%d\n",numero_registradores,a,b);
            numero_registradores++;
        }
        if(tokens[i].id==DIV){
            printf("div $t%d, $t%d, $t%d\n",numero_registradores,a,b);
            numero_registradores++;
        }
        if(tokens[i].id==MUL){
            printf("mul $t%d, $t%d, $t%d\n",numero_registradores,a,b);
            numero_registradores++;
        }
    }
    return 1;

}

int operadores_booleanos(){
    int indice = INDICE;
    next_token();
    if(token.id==OR||token.id==AND){
        return 1;
    }
    reset_token(indice);
    return 0;
}

int operadores_comparacao(){
    int indice = INDICE;
    next_token();
    if(token.id==IGUAL_IGUAL||token.id==DIFERENTE||token.id==MAIOR||token.id==MAIOR_IGUAL||token.id==MENOR||token.id==MENOR_IGUAL){
        return 1;
    }
    reset_token(indice);
    return 0;
}

int operadores_duplos(){
    int indice = INDICE;
    next_token();
    if(token.id==ADD_ADD||token.id==SUB_SUB){
        return 1;
    }
    reset_token(indice);
    return 0;
}
int operadores_composto(){
    int indice = INDICE;
    next_token();
    if(token.id==ADD_IGUAL||token.id==SUB_IGUAL||token.id==MUL_IGUAL||token.id==DIV_IGUAL){
        return 1;
    }
    reset_token(indice);
    return 0;
}

int expressao2(){
    int indice = INDICE;
    next_token();
    if(token.id==IGUAL){
        if(expressao()){
            return 1;
        }
        return erro("expressao");
    }
    reset_token(indice);
    if(operadores_comparacao()){
        if(expressao()){
            return 1;
        }
        return erro("expressao");
    }
    if(operadores()){
        if(expressao()){
            return 1;
        }
        return erro("expressao");
    }
    if(operadores_duplos()){
        return 1;
    }
    if(operadores_composto()){
        if(expressao()){
            return 1;
        }
        return erro("expressao");
    }
    if(operadores_booleanos()){
        if(expressao()){
            return 1;
        }
        return erro("expressao");
    }
    return 0;
}

int expressao(){
    int indice = INDICE;
    next_token();
    if(token.id==ID){
        if(expressao2()){
            return 1;
        }
    }
    reset_token(indice);
    if(expressao_aritimetica()){
        return 1;
    }
    return 0;
}

void ler_tokens(){
  int n=0;
  while(fread(&token,sizeof(Token),1,arq)){
    n++;
  }
  Tamanho = n;
  fseek(arq,0,SEEK_SET);
  tokens = malloc(sizeof(Token)*n);
  n=0;
  while(fread(&token,sizeof(Token),1,arq)){
    tokens[n] = token;
    n++;
  }

}

void last_token(){
  if(debug){
    printf("voltando\n");
  }

  token = tokens[INDICE-2];
  INDICE--;
}

void next_token(){
  if(INDICE==Tamanho){
    token.id = FIM;
    return ;
    }
  token = tokens[INDICE];
  INDICE++;
  if(debug){
    printf("%d\n",token.id);
  }
}

void reset_token(int i){
    if(i != 0){
        token = tokens[i-1];
        INDICE=i;
        return;
    }
    token = tokens[0];
    INDICE=i;
}

int tipo(){
    int indice = INDICE;
    next_token();
    if(token.id==5||token.id==9||token.id==11||token.id==14||token.id==23){
        return token.id;
    }
    reset_token(indice);
    return 0;
}

int identificador(){
    int indice = INDICE;
    next_token();
    if(token.id==0){
        return 1;
    
    }
    reset_token(indice);
    return 0;
}

int lista_declaracao(){
    int indice = INDICE;;
    int t;
    while(1){
        if(t = tipo()){
            if (identificador()){
                if(!(existe_em_lista_var(token.palavra))){
                    adicionar_lista_var(token.palavra,t,numero_registradores);
                    printf("lw $t%d, %s",numero_registradores, token.palavra);
                    
                }
                else{
                    return erro("variavel ja declarada");
                }
                next_token();
                if (token.id==VIRGULA){
                    continue;
                }
                return 1;
            }
            return erro("declaracao");
        }
        reset_token(indice);
        return 1;
    }
}
int lista_parametro_cahamada_funcao(){
    int indice = INDICE;
    while(1){
        if(expressao()){
            next_token();
            if(token.id == VIRGULA){
                continue;
            }
                last_token();
                return 1;
        }
        return(erro("parametros chamada de funcao"));   
    }
    return 1;
}
int lista_ids_com_expressao(int x){
    int indice = INDICE;
    Token t;
    while(1){
        if(identificador()){
            strcpy(t.palavra,token.palavra);
            next_token();
            if(token.id == IGUAL){
                if(expressao()){
                    next_token();
                    if(token.id == VIRGULA){
                        if(!(existe_em_lista_var(t.palavra))){
                            adicionar_lista_var(t.palavra,x,numero_registradores);
                            printf("lw $t%d, %s\n",numero_registradores, token.palavra);
                            numero_registradores++;
                            continue;
                        }
                        else 
                            return erro("variavel ja declarada");
                    }
                    last_token();
                    if(!(existe_em_lista_var(t.palavra))){
                        adicionar_lista_var(t.palavra,x,numero_registradores);
                        printf("sw %s, $t%d\n", t.palavra,numero_registradores);
                        numero_registradores++;
                        return 1;
                    }
                    else 
                        return erro("variavel ja declarada");
                }
                return(erro("Declaracao"));
            }
            if(token.id == VIRGULA){
                continue;
            }
            if(!(existe_em_lista_var(t.palavra))){
                adicionar_lista_var(t.palavra,x,numero_registradores);
                printf("lw $t%d, %s\n",numero_registradores, t.palavra);
                numero_registradores++;
                last_token();
                return 1;
            }
            else 
                return erro("variavel ja declarada");
        }
        reset_token(indice);
        return 0;
    }
}

int declaracao(){
    int indice = INDICE;
    int t;
    if(t = tipo()){
        if (lista_ids_com_expressao(t)){
            return 1;
        }
    }
    reset_token(indice);
    return 0;
}

int bloco_else(int numero_if){
    int indice = INDICE;
    next_token();
    if(token.id==10){
        printf("else%d:\n",numero_if);
        if(bloco()){
            return 1;
        }
        return erro("else");
    }
    reset_token(indice);
    return 1;
}

int bloco_if(){
    int indice = INDICE;
    next_token();
    if(token.id==13){
        next_token();
        if(token.id==O_PARENTESES){
            if(expressao()){
                next_token();
                if(token.id==C_PARENTESES){
                    int nif = numero_ifs;
                    printf("if%d:\n",nif);
                    numero_ifs++;
                    if(bloco()){
                        if(bloco_else(nif)){
                            return 1;
                        }
                    }
                }
                else{
                    return(erro("parenteses if"));
                }
            }
        }
        else{
            return(erro("parenteses if"));
        }
    }
    reset_token(indice);
    return 0;
}

int bloco_escopo(){
    int indice = INDICE;
    next_token();
    if(token.id==O_CHAVE){
        while(bloco());
        next_token();
        if(token.id==C_CHAVE){
            return 1;
        }
        
        return erro("escopo");
    }
    reset_token(indice);
    return 0;
}


int lista_case(){
    int indice = INDICE;
    while(1){
        next_token();
        if(token.id==4){
            if(expressao()){
                next_token();
                if(token.id==DOIS_PONTOS){
                    while(bloco());
                        continue;
                }
                else{
                    return erro("dois pontos switch");
                }
            }
        }
        if(token.id==7){
            next_token();
            if(token.id==DOIS_PONTOS){
                while(bloco())
                    return 1;
            }
            else{
                return erro("dois pontos switch");
            }
        }
        last_token();
        return 1;   
    }
    reset_token(indice);
    return 0;
}

int bloco_switch(){
    int indice = INDICE;
    next_token();
    if(token.id==20){
        next_token();
        if(token.id==O_PARENTESES){
            if(expressao()){
                next_token();
                if(token.id==C_PARENTESES){
                    next_token();
                    if(token.id==O_CHAVE){
                        int loop = numero_loops;
                        printf("loop%d:\n",loop);
                        empilhar_l(loop);
                        if(lista_case()){
                            printf("fim_loop%d:\n",loop);
                            desempilhar_l();
                            next_token();
                            if(token.id==C_CHAVE){
                                return 1;
                            }
                            else 
                                return erro("chave switch");
                        }
                    }
                    else 
                        return erro("chave switch");
                }
                else 
                    return erro("parenteses switch");
            }
        }
         else 
            return erro("parenteses switch");
    }
    reset_token(indice);
    return 0;
}

int bloco_while(){
    int indice = INDICE;
    next_token();
    if(token.id==24){
        next_token();
        if(token.id==O_PARENTESES){
            if(expressao()){
                next_token();
                if(token.id==C_PARENTESES){
                    int loop = numero_loops;
                    printf("loop%d:\n",loop);
                    empilhar_l(loop);
                    if(bloco_escopo()){
                        printf("fim_loop%d:\n",loop);
                        desempilhar_l();
                        return 1;
                    }
                }
                else{
                    return(erro("parenteses while"));
                }
            }
        }
        else{
            return(erro("parenteses while"));
        }
    }
    reset_token(indice);
    return 0;
}


int bloco_for(){
    int indice = INDICE;;
    next_token();
    if(token.id==12){
        next_token();
        if(token.id==O_PARENTESES){
            if(expressao()){
                next_token();
                if(token.id==PONTO_VIRGULA){
                    if(expressao()){
                        next_token();
                        if(token.id==PONTO_VIRGULA){
                            if(expressao()){
                                next_token();
                                if(token.id==C_PARENTESES){
                                    int loop = numero_loops;
                                    printf("loop%d:\n",loop);
                                    empilhar_l(loop);
                                    if(bloco_escopo()){
                                        printf("fim_loop%d:\n",loop);
                                        desempilhar_l();
                                        return 1;
                                    }
                                }
                                else{
                                    return(erro("parenteses for"));
                                }
                            }
                        }
                        else{
                            return(erro("ponto e virgula for"));
                        }
                    }
                }
                else{
                    return(erro("ponto e vigula for"));
                }
            }
        }
        else{
            return(erro("parenteses for"));
        }
    }
    reset_token(indice);
    return 0;

}

int bloco_return(){
    int indice = INDICE;
    if(expressao()){
        return 1;
    }
    reset_token(indice);
    return 1;
}

int bloco(){
    int indice = INDICE;
    if(bloco_if()||bloco_while()||bloco_for()||bloco_escopo()||bloco_switch()){
        return 1;
    }
    if(declaracao()||expressao()){
        next_token();
        if(token.id==PONTO_VIRGULA){
            return 1;
        }
        return erro("bloco");
    }
    next_token();
    if(token.id == PONTO_VIRGULA){
        return 1;
    }
    if(token.id==16){
        if(bloco_return()){
            next_token();
            if(token.id==PONTO_VIRGULA){
                return 1;
            }
            else 
                return erro("ponto e virgula return");
        }
    }
    if(token.id==3){
        next_token();
        if(token.id==PONTO_VIRGULA){
            if (p.topo!=NULL){
                printf("j fim_loop%d\n",p.topo->n);
                return 1;
            }
            else
                return erro("break fora do loop");
        }
        else{
            return erro("ponto e virgula break");
        }
    }
    if(token.id==6){
        next_token();
        if(token.id==PONTO_VIRGULA){
            if (p.topo!=NULL){
                printf("j loop%d\n",p.topo->n);
                return 1;
            }
            else
                return erro("continue fora do loop");
        }
        else{
            return erro("ponto e virgula continue");
        }
    }
    reset_token(indice);
    return 0;
}

int funcao(){
    int indice = INDICE;
    Token t;
    if(tipo()){
        if(identificador()){
            empilhar_escopo(token.palavra);
            strcpy(t.palavra,token.palavra);
            next_token();
            if(token.id==O_PARENTESES){
                if(lista_declaracao()){
                    next_token();
                    if(token.id==C_PARENTESES){
                        next_token();
                        if(token.id==O_CHAVE){
                            printf("%s:\n",t.palavra);
                            while(bloco());
                            next_token();
                            if(token.id==C_CHAVE){
                            if(strcmp(t.palavra,"main")!=0){
                                 adicionar_lista_var(t.palavra,FUNCAO,0);
                            }
                                desempilhar_escopo();
                                return 1;
                            }
                            else{
                                return erro("chaves f1");
                            }
                        }
                        else{
                            return erro("chaves f2");
                        }
                    }
                    else{
                        return erro("parenteses");
                    }        
                }
            }
            else{
                return erro("parenteses");
            } 
            desempilhar_escopo();
        }
    }
    
    reset_token(indice);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc == 3){
      if (strcmp(argv[2],"d")==0){
        debug = 1;
      }
    }
    gera_tokens(argv[1]);
    n_erros=0;
    arq = fopen("lista_tokens","rb");
    ler_tokens();
    criar_lista_var("main",FUNCAO);
    int result;
    while(result = funcao());
    if(n_erros==0){
      printf("OK\n");
    }
    else{
      printf("Numero de erros: %d\n",n_erros);
    }
    free(tokens);
    fclose(arq);
}
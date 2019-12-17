#include "lexico.h"

Token token;

const char palavras_reservadas[22][10] = {
"break" //3
,"case" //4
,"char" //5
,"continue"//6
,"default"//7
,"do"//8
,"double"//9
,"else"//10
,"float"//11
,"for"//12
,"if"//13
,"int"//14
,"long"//15
,"return"//16
,"short"//17
,"sizeof"//18
,"struct"//19
,"switch"//20
,"typedef"//21
,"unsigned"//22
,"void"//23
,"while" };//24

void gera_token_palavra(char palavra[], int coluna, int linha, FILE * arq) {
    int i;
    for (i = 0; i < 22; i++) {
        if (strcmp(palavra, palavras_reservadas[i]) == 0) {
            token.id = i + 3;
            token.coluna = coluna;
            token.linha = linha;
            fwrite( & token, sizeof(token), 1, arq);
            return;
        }
    }
    token.id = ID;
    strncpy((char * ) & token.palavra, palavra, MAX_PALAVRA);
    token.coluna = coluna+1;
    token.linha = linha+1;
    fwrite( & token, sizeof(token), 1, arq);

    return;
}

void gera_token_separador(char c, int coluna, int linha, FILE * arq) {
    switch (c) {
    case ';':
        token.id = PONTO_VIRGULA;
        break;
    case '[':
        token.id = O_COLCHETE;
        break;
    case ']':
        token.id = C_COLCHETE;
        break;
    case ',':
        token.id = VIRGULA;
        break;
    case '(':
        token.id = O_PARENTESES;
        break;
    case ')':
        token.id = C_PARENTESES;
        break;
    case '\"':
        token.id = ASPAS_D;
        break;
    case '\'':
        token.id = ASPAS_S;
        break;
    case '{':
        token.id = O_CHAVE;
        break;
    case '}':
        token.id = C_CHAVE;
        break;
    case ':':
        token.id = DOIS_PONTOS;
        break;
    }
    token.linha = linha+1;
    token.coluna = coluna+1;
    fwrite( & token, sizeof(token), 1, arq);
}

void gera_token_numero(char numero[], int coluna, int linha, FILE * arq) {
    strncpy((char * ) & token.numero, numero, MAX_NUMERO);
    token.id = NUM;
    token.linha = linha+1;
    token.coluna = coluna+1;
    fwrite( & token, sizeof(token), 1, arq);
}

void gera_token_operador(char c[], int coluna, int linha, FILE * arq) {
    if (strcmp(c, "+") == 0) {
        token.id = ADD;
    } else if (strcmp(c, "-") == 0) {
        token.id = SUB;
    } else if (strcmp(c, "*") == 0) {
        token.id = MUL;
    } else if (strcmp(c, "/") == 0) {
        token.id = DIV;
    } else if (strcmp(c, "=") == 0) {
        token.id = IGUAL;
    } else if (strcmp(c, "||") == 0) {
        token.id = OR;
    } else if (strcmp(c, "&&") == 0) {
        token.id = AND;
    } else if (strcmp(c, "++") == 0) {
        token.id = ADD_ADD;
    } else if (strcmp(c, "--") == 0) {
        token.id = SUB_SUB;
    } else if (strcmp(c, "+=") == 0) {
        token.id = ADD_IGUAL;
    } else if (strcmp(c, "*=") == 0) {
        token.id = MUL_IGUAL;
    } else if (strcmp(c, "/=") == 0) {
        token.id = DIV_IGUAL;
    } else if (strcmp(c, "-=") == 0) {
        token.id = SUB_IGUAL;
    } else if (strcmp(c, "==") == 0) {
        token.id = IGUAL_IGUAL;
    } else if (strcmp(c, ">") == 0) {
        token.id = MAIOR;
    } else if (strcmp(c, "<") == 0) {
        token.id = MENOR;
    } else if (strcmp(c, ">=") == 0) {
        token.id = MAIOR_IGUAL;
    } else if (strcmp(c, "<=") == 0) {
        token.id = MENOR_IGUAL;
    } else if (strcmp(c, "!=") == 0) {
        token.id = DIFERENTE;
    }
    token.linha = linha+1;
    token.coluna = coluna+1;
    fwrite( & token, sizeof(token), 1, arq);
    return;
}

void gera_token_literal(char str[], FILE * arq) {
    token.id = LITERAL;
    strncpy((char * ) & token.literal, str, MAX_LITERAL);
    fwrite( & token, sizeof(token), 1, arq);

}

void gera_tokens(char * filename) {
    FILE * arq = fopen(filename, "r");
    FILE * erros = fopen("logErros.txt", "w");
    FILE * lista_tokens = fopen("lista_tokens", "wb");
    int coluna = 0, linha = 0, i;
    char palavra[MAX_PALAVRA];
    char numero[MAX_NUMERO];
    char c = fgetc(arq);
    char literal[MAX_LITERAL];
    char operador[3];
    while (1) {
        //-----------------------Palavras(reservadas ou nao)-------------------------------------
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' ||
            c == '$') {
            for (i = 0;(c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '$';i++) {
                palavra[i] = c;
                c = fgetc(arq);
                coluna++;
            }
            palavra[i] = '\0';
            gera_token_palavra(palavra, coluna, linha, lista_tokens);
        }
        //-----------------------Numeros-------------------------------------
        else if (c >= '0' && c <= '9') {
            i = 0;
            if (c == '0') {
                numero[i] = c;
                c = fgetc(arq);
                coluna++;
                i++;
                if (c == 'x') {
                    numero[i] = c;
                    for (;(c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); i++) {
                        numero[i] = c;
                        c = fgetc(arq);
                        coluna++;
                    }
                }
            }
            for (;(c >= '0' && c <= '9') || c == '.'; i++) {
                numero[i] = c;
                c = fgetc(arq);
                coluna++;
            }
            numero[i] = '\0';
            gera_token_numero(numero, coluna, linha, lista_tokens);
        } else if (c == '.') {
            i = 2;
            numero[0] = '0';
            numero[1] = '.';
            c = fgetc(arq);
            for (;
                (c >= '0' && c <= '9'); i++) {
                numero[i] = c;
                c = fgetc(arq);
                coluna++;
            }
            numero[i] = '\0';
            gera_token_numero(numero, coluna, linha, lista_tokens);
        }
        //-----------------------Literais-------------------------------------
        else if (c == '\'') {
            gera_token_separador(c, coluna, linha, lista_tokens);
            c = fgetc(arq);
            coluna++;
            for (i = 0; i < MAX_LITERAL; i++) {
                if (c =='\\'){
                    c = fgetc(arq);
                }
                else if (c == '\'') {
                    break;
                }
                literal[i] = c;
                c = fgetc(arq);
                coluna++;
            }
            literal[i] = '\0';
            gera_token_literal(literal, lista_tokens);
            gera_token_separador(c, coluna, linha, lista_tokens);
            c = fgetc(arq);
            coluna++;
        } else if (c == '\"') {
            gera_token_separador(c, coluna, linha, lista_tokens);
            c = fgetc(arq);
            coluna++;
            for (i = 0; i < MAX_LITERAL; i++) {
                if (c =='\\'){
                    c = fgetc(arq);
                }
                else if (c == '\"') {
                    break;
                }
                literal[i] = c;
                c = fgetc(arq);
                coluna++;
            }
            literal[i] = '\0';
            gera_token_literal(literal, lista_tokens);
            gera_token_separador(c, coluna, linha, lista_tokens);
            c = fgetc(arq);
            coluna++;
        }
        //------------------------Comentarios-----------------------------------
        else if (c == '/') {
            c = fgetc(arq);
            coluna++;
            if (c == '/') {
                while (1) {
                    c = fgetc(arq);
                    coluna++;
                    if (c == '\n') {
                        coluna = 0;
                        linha++;
                        c = fgetc(arq);
                        break;
                    }
                }
            } else if (c == '*') {
                c = fgetc(arq);
                if (c == '\n') {
                    coluna = 0;
                    linha++;
                } else
                    coluna++;
                while (1) {
                    if (c == '*') {
                        c = fgetc(arq);
                        if (c == '\n') {
                            coluna = 0;
                            linha++;
                        } else
                            coluna++;

                        if (c == '/') {
                            break;
                        } else
                            continue;
                    }
                    c = fgetc(arq);
                    if (c == '\n') {
                        coluna = 0;
                        linha++;
                    } else
                        coluna++;
                }
            } else if (c == '=') {
                gera_token_operador("/=", coluna, linha, lista_tokens);
            } else
                gera_token_operador("/", coluna, linha, lista_tokens);
        }
        //---------------------------Separadores--------------------------
        else if (c == ';' || c == '[' || c == ']' || c == ',' || c == '(' ||
            c == ')' || c == '{' || c == '}' || c == ':') {
            gera_token_separador(c, coluna, linha, lista_tokens);
            c = fgetc(arq);
            coluna++;
        }
        //---------------------------Operadores----------------------------
        else if (c == '*' || c == '+' || c == '-' || c == '=' || c == '<' ||c == '>' || c == '!') {
            operador[0] = c;
            c = fgetc(arq);
            coluna++;
            if (c == '=') {
                operador[1] = c;
                c = fgetc(arq);
                coluna++;
            } else if (operador[0] == '+' && c == '+') {
                operador[1] = c;
                c = fgetc(arq);
                coluna++;
            } else if (operador[0] == '-' && c == '-') {
                operador[1] = c;
                c = fgetc(arq);
                coluna++;
            } else {
                operador[1] = '\0';
            }
            operador[2] = '\0';
            gera_token_operador(operador, coluna - 1, linha, lista_tokens);
        } else if (c == '|' || c == '&') {
            operador[0] = c;
            c = fgetc(arq);
            coluna++;
            if (operador[0] == '|' && c == '|') {
                operador[1] = c;
                c = fgetc(arq);
                coluna++;
                operador[2] = '\0';
                gera_token_operador(operador, coluna - 1, linha, lista_tokens);
            } else if (operador[0] == '&' && c == '&') {
                operador[1] = c;
                c = fgetc(arq);
                coluna++;
                operador[2] = '\0';
                gera_token_operador(operador, coluna - 1, linha, lista_tokens);
            } else {
                fprintf(erros, "Erro lexico linha: %d, coluna: %d.\n", linha,
                    coluna);
            }
        } else if (c == '\n') {
            c = fgetc(arq);
            coluna = 0;
            linha++;
        } else if (c == EOF) {
            break;
        } else if (c == ' ') {
            c = fgetc(arq);
            coluna++;
        } else {
            fprintf(erros,
                "Simbolo %c nao reconhecido, linha: %d, coluna: %d.\n", c,
                linha, coluna);
            c = fgetc(arq);
            coluna++;
        }
    }
    fclose(arq);
    fclose(erros);
    fclose(lista_tokens);
}

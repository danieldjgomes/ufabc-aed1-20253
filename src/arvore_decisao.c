#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_decisao.h"

NoDecisao* criar_no_decisao(TipoNo tipo, float valor, NoDecisao* esq, NoDecisao* dir) {
    NoDecisao* no = (NoDecisao*) malloc(sizeof(NoDecisao));
    if (no != NULL) {
        no->tipo = tipo;
        no->valor_corte = valor;
        no->categoria[0] = '\0';
        no->esquerda = esq;
        no->direita = dir;
    }
    return no;
}

NoDecisao* criar_folha_categoria(const char* categoria) {
    NoDecisao* no = (NoDecisao*) malloc(sizeof(NoDecisao));
    if (no != NULL) {
        no->tipo = TIPO_CATEGORIA;
        no->valor_corte = 0;
        strncpy(no->categoria, categoria, sizeof(no->categoria) - 1);
        no->categoria[sizeof(no->categoria) - 1] = '\0';
        no->esquerda = NULL;
        no->direita = NULL;
    }
    return no;
}

void liberar_arvore_decisao(NoDecisao* raiz) {
    if (raiz != NULL) {
        liberar_arvore_decisao(raiz->esquerda);
        liberar_arvore_decisao(raiz->direita);
        free(raiz);
    }
}

float get_consumo_minimo_categoria(const char* categoria) {
    if (strcmp(categoria, "Platinum") == 0) return 1001.0; 
    if (strcmp(categoria, "Gold") == 0) return 1001.0;     
    if (strcmp(categoria, "Silver") == 0) return 501.0;  
    if (strcmp(categoria, "Bronze") == 0) return 501.0; 
    if (strcmp(categoria, "Standard") == 0) return 0.0;
    return 0.0;
}

const char* get_categoria_anterior(const char* categoria) {
    if (strcmp(categoria, "Platinum") == 0) return "Gold";
    if (strcmp(categoria, "Gold") == 0) return "Silver";
    if (strcmp(categoria, "Silver") == 0) return "Bronze";
    if (strcmp(categoria, "Bronze") == 0) return "Standard";
    return "Standard";
}

void aplicar_regras_arvore(NoDecisao* raiz, Cliente* c) {
    if (raiz == NULL) return;

    if (raiz->tipo == TIPO_CATEGORIA) {
        strcpy(c->categoria, raiz->categoria);
        return;
    }

    int condicao = 0;
    if (raiz->tipo == TIPO_VISITAS_MES_ANTERIOR) {
        condicao = (c->visitas_mes_anterior < raiz->valor_corte);
    } else if (raiz->tipo == TIPO_CONSUMO_MES_ANTERIOR) {
        condicao = (c->consumo_mes_anterior < raiz->valor_corte);
    }

    if (condicao) {
        aplicar_regras_arvore(raiz->esquerda, c);
    } else {
        aplicar_regras_arvore(raiz->direita, c);
    }
}

void classificar_cliente_mensal(NoDecisao* raiz, Cliente* c, int mes_atual, int ano_atual) {
    (void)mes_atual; 
    (void)ano_atual;
    aplicar_regras_arvore(raiz, c);
}

#ifndef ARVORE_DECISAO_H
#define ARVORE_DECISAO_H

#include "cliente.h"

typedef enum {
    TIPO_CONSUMO_MES_ANTERIOR,
    TIPO_VISITAS_MES_ANTERIOR,
    TIPO_CATEGORIA // Folha
} TipoNo;

typedef struct NoDecisao {
    TipoNo tipo;
    float valor_corte; // Usado para CONSUMO e FREQUENCIA
    char categoria[50]; // Usado apenas para TIPO_CATEGORIA
    struct NoDecisao *esquerda; // Caminho se (atributo < valor_corte) for VERDADEIRO
    struct NoDecisao *direita;  // Caminho se (atributo < valor_corte) for FALSO
} NoDecisao;

NoDecisao* criar_no_decisao(TipoNo tipo, float valor, NoDecisao* esq, NoDecisao* dir);
NoDecisao* criar_folha_categoria(const char* categoria);
void liberar_arvore_decisao(NoDecisao* raiz);

// Updated classification function with downgrade logic
void classificar_cliente_mensal(NoDecisao* raiz, Cliente* c, int novo_mes, int novo_ano);

// Helper to get min consumption for a category
float get_consumo_minimo_categoria(const char* categoria);
// Helper to get the previous category
const char* get_categoria_anterior(const char* categoria);

#endif

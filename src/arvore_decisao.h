#ifndef ARVORE_DECISAO_H
#define ARVORE_DECISAO_H

#include "categoria.h"

typedef enum
{
	TIPO_CONSUMO_MES_ANTERIOR,
	TIPO_VISITAS_MES_ANTERIOR,
	TIPO_CATEGORIA // Folha
} TipoNo;

typedef struct NoDecisao
{
	TipoNo tipo;
	float valor_corte;			// Usado para CONSUMO e FREQUENCIA
	Categoria categoria;		// Usado apenas para TIPO_CATEGORIA
	struct NoDecisao *esquerda; // Caminho se (atributo < valor_corte) for VERDADEIRO
	struct NoDecisao *direita;	// Caminho se (atributo < valor_corte) for FALSO
} NoDecisao;

NoDecisao *criar_no_decisao(TipoNo tipo, float valor, NoDecisao *esq, NoDecisao *dir);

NoDecisao *criar_folha_categoria(Categoria);

void liberar_arvore_decisao(NoDecisao *raiz);

// Helper to get min consumption for a category
float get_consumo_minimo_categoria(Categoria);

#endif

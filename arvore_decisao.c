#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore_decisao.h"
#include "cliente.h"
#include "categoria.h"

NoDecisao *criar_no_decisao(TipoNo tipo, float valor, NoDecisao *esq, NoDecisao *dir)
{
	NoDecisao *no = (NoDecisao *)malloc(sizeof(NoDecisao));
	if (no != NULL)
	{
		no->tipo = tipo;
		no->valor_corte = valor;
		no->categoria = STANDARD;
		no->esquerda = esq;
		no->direita = dir;
	}
	return no;
}

NoDecisao *criar_folha_categoria(Categoria categoria)
{
	NoDecisao *no = (NoDecisao *)malloc(sizeof(NoDecisao));
	if (no != NULL)
	{
		no->tipo = TIPO_CATEGORIA;
		no->valor_corte = 0;
		no->categoria = categoria;
		no->esquerda = NULL;
		no->direita = NULL;
	}
	return no;
}

void liberar_arvore_decisao(NoDecisao *raiz)
{
	if (raiz != NULL)
	{
		liberar_arvore_decisao(raiz->esquerda);
		liberar_arvore_decisao(raiz->direita);
		free(raiz);
	}
}

float get_consumo_minimo_categoria(Categoria categoria)
{
	switch (categoria)
	{
	case PLATINUM:
	case GOLD:
		return 1001.0;
	case SILVER:
	case BRONZE:
		return 501.0;
	case STANDARD:
		return 0.0;
	default:
		return 0.0;
	}
}

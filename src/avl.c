#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

static NoAVL *reequilibrar(NoAVL *);
static int max(int, int);
static int avl_altura(NoAVL *no);
static int avl_fator_balanceamento(NoAVL *no);

NoAVL *avl_criar()
{
	return NULL;
}

void avl_liberar(NoAVL *raiz)
{
	if (raiz != NULL)
	{
		avl_liberar(raiz->esquerda);
		avl_liberar(raiz->direita);
		destruir_cliente(raiz->cliente);
		free(raiz);
	}
}

void update_height(NoAVL *no)
{
	if (no != NULL)
	{
		no->altura = 1 + max(avl_altura(no->esquerda), avl_altura(no->direita));
	}
}

NoAVL *rotacao_direita(NoAVL *y)
{
	if (y == NULL)
	{
		return NULL;
	}

	NoAVL *pivo = y->esquerda;
	y->esquerda = pivo->direita;
	pivo->direita = y;

	update_height(y);
	update_height(pivo);

	return pivo;
}

NoAVL *rotacao_esquerda(NoAVL *y)
{
	if (y == NULL)
	{
		return NULL;
	}

	NoAVL *pivo = y->direita;
	y->direita = pivo->esquerda;
	pivo->esquerda = y;

	update_height(y);
	update_height(pivo);

	return pivo;
}

NoAVL *avl_inserir(NoAVL *no, Cliente *c, NoDecisao *regras)
{
	if (no == NULL)
	{
		NoAVL *novo = (NoAVL *)malloc(sizeof(NoAVL));
		novo->cliente = c;
		novo->esquerda = NULL;
		novo->direita = NULL;
		novo->altura = 0;
		return novo;
	}

	if (c->id < no->cliente->id)
		no->esquerda = avl_inserir(no->esquerda, c, regras);
	else
		no->direita = avl_inserir(no->direita, c, regras);

	return reequilibrar(no);
}

NoAVL *avl_buscar(NoAVL *raiz, int id)
{
	if (raiz == NULL || raiz->cliente->id == id)
		return raiz;

	if (id < raiz->cliente->id)
		return avl_buscar(raiz->esquerda, id);

	return avl_buscar(raiz->direita, id);
}

void avl_imprimir_em_ordem(NoAVL *raiz)
{
	if (raiz != NULL)
	{
		avl_imprimir_em_ordem(raiz->esquerda);
		imprimir_cliente(raiz->cliente);
		avl_imprimir_em_ordem(raiz->direita);
	}
}

NoAVL *avl_atualizar_nome(NoAVL *raiz, int id, const char *novo_nome)
{
	NoAVL *no = avl_buscar(raiz, id);
	if (no != NULL)
	{
		strncpy(no->cliente->nome, novo_nome, sizeof(no->cliente->nome) - 1);
		no->cliente->nome[sizeof(no->cliente->nome) - 1] = '\0';
	}
	return raiz;
}

NoAVL *avl_realizar_compra(NoAVL *raiz, int id, float valor, int mes, int ano, NoDecisao *regras)
{
	NoAVL *no = avl_buscar(raiz, id);
	if (no != NULL)
	{
		no->cliente->consumo_mes_atual += valor;
		no->cliente->visitas_mes_atual++;
	}
	return raiz;
}

static NoAVL *reequilibrar(NoAVL *a)
{
	if (a == NULL)
	{
		return NULL;
	}

	update_height(a);

	int fb = avl_fator_balanceamento(a);

	// Caso 1: Desbalanceamento para a direita (Rotação Esquerda)
	if (fb < -1)
	{
		if (avl_fator_balanceamento(a->direita) > 0)
		{
			a->direita = rotacao_direita(a->direita);
		}
		return rotacao_esquerda(a);
	}

	// Caso 2: Desbalanceamento para a esquerda (Rotação Direita)
	if (fb > 1)
	{
		if (avl_fator_balanceamento(a->esquerda) < 0)
		{
			a->esquerda = rotacao_esquerda(a->esquerda);
		}
		return rotacao_direita(a);
	}

	// Caso 3: Nenhum desbalanceamento (Sem rotação)
	return a;
}

static int max(int a, int b)
{
	return (a > b) ? a : b;
}

static int avl_altura(NoAVL *no)
{
	if (no == NULL)
		return -1;
	return no->altura;
}

static int avl_fator_balanceamento(NoAVL *no)
{
	if (no == NULL)
		return 0;
	return avl_altura(no->esquerda) - avl_altura(no->direita);
}

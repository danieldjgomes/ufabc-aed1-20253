#ifndef AVL_H
#define AVL_H

#include "cliente.h"
#include "arvore_decisao.h"

typedef struct NoAVL NoAVL;

struct NoAVL
{
	Cliente *cliente;
	int altura;
	struct NoAVL *esquerda;
	struct NoAVL *direita;
};

NoAVL *avl_criar();

void avl_liberar(NoAVL *raiz);

NoAVL *avl_inserir(NoAVL *raiz, Cliente *c, NoDecisao *regras);

NoAVL *avl_buscar(NoAVL *raiz, int id);

void avl_imprimir_em_ordem(NoAVL *raiz);

NoAVL *avl_atualizar_nome(NoAVL *raiz, int id, const char *novo_nome);

NoAVL *avl_realizar_compra(NoAVL *raiz, int id, float valor, int mes, int ano, NoDecisao *regras);

#endif

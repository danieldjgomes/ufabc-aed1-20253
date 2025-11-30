#include <stdio.h>
#include <stdlib.h>

#include "../src/avl.h"
#include "../src/cliente.h"
#include "../src/categoria.h"

int test_avl_inserir()
{
	printf("Testando inserção na árvore AVL...\n");

	Cliente *c1 = criar_cliente("A");
	Cliente *c2 = criar_cliente("B");
	Cliente *c3 = criar_cliente("C");

	NoAVL *r = avl_criar();
	r = avl_inserir(r, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);

	if (r == NULL || r->cliente == NULL)
	{
		printf("%s falhou na criação da estrutura da árvore AVL\n", __func__);
		return 0;
	}

	NoAVL *no_c1 = avl_buscar(r, c1->id);
	NoAVL *no_c2 = avl_buscar(r, c2->id);
	NoAVL *no_c3 = avl_buscar(r, c3->id);

	int result = 1;
	result = (no_c1 != NULL && no_c1->cliente != NULL && no_c1->cliente->id == c1->id) ? 1 : 0;
	result = (no_c2 != NULL && no_c2->cliente != NULL && no_c2->cliente->id == c2->id) ? 1 : 0;
	result = (no_c3 != NULL && no_c3->cliente != NULL && no_c3->cliente->id == c3->id) ? 1 : 0;
	if (result == 0)
	{
		printf("%s falhou na busca pelos clientes inseridos na AVL\n", __func__);
	}

	return result;
}

int test_avl_structure_after_right_rotation()
{
	printf("Testando estrutura da árvore AVL depois de rotação para a direita...\n");

	Cliente *c1 = criar_cliente_with_id("A", 10);
	Cliente *c2 = criar_cliente_with_id("B", 7);
	Cliente *c3 = criar_cliente_with_id("C", 20);
	Cliente *c4 = criar_cliente_with_id("D", 5);
	Cliente *c5 = criar_cliente_with_id("E", 9);
	Cliente *c6 = criar_cliente_with_id("F", 3);

	NoAVL *r = avl_inserir(NULL, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);
	r = avl_inserir(r, c4, NULL);
	r = avl_inserir(r, c5, NULL);
	r = avl_inserir(r, c6, NULL);

	int result = 1;
	if (r->cliente->id != 7)
		result = 0;
	if (r->esquerda->cliente->id != 5)
		result = 0;
	if (r->esquerda->esquerda->cliente->id != 3)
		result = 0;
	if (r->direita->cliente->id != 10)
		result = 0;
	if (r->direita->direita->cliente->id != 20)
		result = 0;
	if (r->direita->esquerda->cliente->id != 9)
		result = 0;
	if (!result)
	{
		printf("%s falhou\n", __func__);
	}
	return result;
}

int test_avl_structure_after_left_rotation()
{
	printf("Testando estrutura da árvore AVL depois de rotação para a esquerda...\n");

	Cliente *c1 = criar_cliente_with_id("A", 10);
	Cliente *c2 = criar_cliente_with_id("B", 5);
	Cliente *c3 = criar_cliente_with_id("C", 20);
	Cliente *c4 = criar_cliente_with_id("D", 15);
	Cliente *c5 = criar_cliente_with_id("E", 25);
	Cliente *c6 = criar_cliente_with_id("F", 30);

	NoAVL *r = avl_inserir(NULL, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);
	r = avl_inserir(r, c4, NULL);
	r = avl_inserir(r, c5, NULL);
	r = avl_inserir(r, c6, NULL);

	int result = 1;
	if (r->cliente->id != 20)
		result = 0;
	if (r->esquerda->cliente->id != 10)
		result = 0;
	if (r->esquerda->esquerda->cliente->id != 5)
		result = 0;
	if (r->esquerda->direita->cliente->id != 15)
		result = 0;
	if (r->direita->cliente->id != 25)
		result = 0;
	if (r->direita->direita->cliente->id != 30)
		result = 0;
	if (result == 0)
	{
		printf("%s falhou\n", __func__);
	}
	return result;
}

int test_avl_structure_after_left_right_rotation()
{
	printf("Testando estrutura da árvore AVL depois de rotação esquerda-direita...\n");

	Cliente *c1 = criar_cliente_with_id("A", 15);
	Cliente *c2 = criar_cliente_with_id("B", 10);
	Cliente *c3 = criar_cliente_with_id("C", 20);
	Cliente *c4 = criar_cliente_with_id("D", 5);
	Cliente *c5 = criar_cliente_with_id("E", 13);
	Cliente *c6 = criar_cliente_with_id("F", 12);

	NoAVL *r = avl_inserir(NULL, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);
	r = avl_inserir(r, c4, NULL);
	r = avl_inserir(r, c5, NULL);
	r = avl_inserir(r, c6, NULL);

	printf("%s - Árvore AVL construída.\n", __func__);

	int result = 1;
	if (r->cliente->id != 13)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 13 está OK.\n", __func__);
	}
	if (r->esquerda->cliente->id != 10)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 10 está OK.\n", __func__);
	}
	if (r->esquerda->esquerda->cliente->id != 5)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 5 está OK.\n", __func__);
	}
	if (r->esquerda->direita->cliente->id != 12)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 12 está OK.\n", __func__);
	}
	if (r->direita->cliente->id != 15)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 15 está OK.\n", __func__);
	}
	if (r->direita->direita->cliente->id != 20)
	{
		result = 0;
	}
	else
	{
		printf("%s - Nó 20 está OK.\n", __func__);
	}
	if (result == 0)
	{
		printf("%s falhou\n", __func__);
	}
	return 1;
}

int test_avl_structure_after_right_left_rotation()
{
	printf("Testando estrutura da árvore AVL depois de rotação direita-esquerda...\n");

	Cliente *c1 = criar_cliente_with_id("A", 15);
	Cliente *c2 = criar_cliente_with_id("B", 10);
	Cliente *c3 = criar_cliente_with_id("C", 30);
	Cliente *c4 = criar_cliente_with_id("D", 25);
	Cliente *c5 = criar_cliente_with_id("E", 40);
	Cliente *c6 = criar_cliente_with_id("F", 29);

	NoAVL *r = avl_inserir(NULL, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);
	r = avl_inserir(r, c4, NULL);
	r = avl_inserir(r, c5, NULL);
	r = avl_inserir(r, c6, NULL);

	int result = 1;
	if (r->cliente->id != 25)
		result = 0;
	if (r->esquerda->cliente->id != 15)
		result = 0;
	if (r->esquerda->esquerda->cliente->id != 10)
		result = 0;
	if (r->direita->cliente->id != 30)
		result = 0;
	if (r->direita->direita->cliente->id != 40)
		result = 0;
	if (r->direita->esquerda->cliente->id != 29)
		result = 0;
	if (result == 0)
	{
		printf("%s falhou\n", __func__);
	}
	return result;
}

int main()
{
	int result = 1;
	result = test_avl_inserir();
	result = test_avl_structure_after_left_rotation();
	result = test_avl_structure_after_right_rotation();
	result = test_avl_structure_after_left_right_rotation();
	result = test_avl_structure_after_right_left_rotation();
	if (result == 1)
	{
		printf("Todos os testes passaram!\n");
	}
	else
	{
		printf("Alguns testes falharam.\n");
	}
	return 0;
}

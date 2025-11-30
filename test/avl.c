#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/avl.h"
#include "../src/cliente.h"
#include "../src/categoria.h"
#include "test_utils.h"

int test_avl_inserir()
{
	print_header("Testando inserção na árvore AVL");

	print_action("Criando clientes A, B, C...");
	Cliente *c1 = criar_cliente("A");
	Cliente *c2 = criar_cliente("B");
	Cliente *c3 = criar_cliente("C");

	print_action("Inserindo clientes na AVL...");
	NoAVL *r = avl_criar();
	r = avl_inserir(r, c1, NULL);
	r = avl_inserir(r, c2, NULL);
	r = avl_inserir(r, c3, NULL);

	int criacao_ok = (r != NULL && r->cliente != NULL);
	print_check(criacao_ok, "Estrutura da árvore AVL criada com sucesso");
	if (!criacao_ok) return 0;

	print_action("Buscando clientes inseridos...");
	NoAVL *no_c1 = avl_buscar(r, c1->id);
	NoAVL *no_c2 = avl_buscar(r, c2->id);
	NoAVL *no_c3 = avl_buscar(r, c3->id);

	int busca_ok = 1;
	if (no_c1 == NULL || no_c1->cliente == NULL || no_c1->cliente->id != c1->id) busca_ok = 0;
	if (no_c2 == NULL || no_c2->cliente == NULL || no_c2->cliente->id != c2->id) busca_ok = 0;
	if (no_c3 == NULL || no_c3->cliente == NULL || no_c3->cliente->id != c3->id) busca_ok = 0;

	print_check(busca_ok, "Todos os clientes foram encontrados na AVL");

	return busca_ok;
}

int test_avl_atualizar_nome()
{
	print_header("Testando atualização de nome na AVL");

	print_action("Criando cliente 'Nome Antigo' (ID 100)...");
	Cliente *c = criar_cliente_with_id("Nome Antigo", 100);

	print_action("Inserindo cliente na árvore...");
	NoAVL *r = avl_criar();
	r = avl_inserir(r, c, NULL);

	print_action("Atualizando nome para 'Nome Novo'...");
	avl_atualizar_nome(r, 100, "Nome Novo");

	print_action("Buscando cliente para verificar atualização...");
	NoAVL *no = avl_buscar(r, 100);

	int result = 0;
	if (no != NULL && no->cliente != NULL)
	{
		if (strcmp(no->cliente->nome, "Nome Novo") == 0)
		{
			result = 1;
		}
		else
		{
			printf("      Esperado: 'Nome Novo' | Recebido: '%s'\n", no->cliente->nome);
		}
	}
	else
	{
		printf("      Cliente não encontrado na árvore.\n");
	}

	print_check(result, "Nome do cliente atualizado corretamente");
	return result;
}

int test_avl_busca_vazia()
{
	print_header("Testando busca em árvore vazia");

	print_action("Criando árvore vazia...");
	NoAVL *r = avl_criar(); // Retorna NULL

	print_action("Tentando buscar ID 1...");
	NoAVL *res = avl_buscar(r, 1);

	int ok = (res == NULL);
	print_check(ok, "Busca em árvore vazia retornou NULL (Segurança)");
	
	if (!ok) printf("      Falha: Retornou ponteiro não nulo ou crashou.\n");
	
	return ok;
}

int test_avl_structure_after_right_rotation()
{
	print_header("Testando estrutura AVL após rotação direita");

	print_action("Inserindo clientes para forçar rotação direita...");
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

	print_action("Verificando estrutura da árvore...");
	int result = 1;
	if (r->cliente->id != 7) result = 0;
	if (r->esquerda->cliente->id != 5) result = 0;
	if (r->esquerda->esquerda->cliente->id != 3) result = 0;
	if (r->direita->cliente->id != 10) result = 0;
	if (r->direita->direita->cliente->id != 20) result = 0;
	if (r->direita->esquerda->cliente->id != 9) result = 0;

	print_check(result, "Estrutura correta após rotação direita");

	return result;
}

int test_avl_structure_after_left_rotation()
{
	print_header("Testando estrutura AVL após rotação esquerda");

	print_action("Inserindo clientes para forçar rotação esquerda...");
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

	print_action("Verificando estrutura da árvore...");
	int result = 1;
	if (r->cliente->id != 20) result = 0;
	if (r->esquerda->cliente->id != 10) result = 0;
	if (r->esquerda->esquerda->cliente->id != 5) result = 0;
	if (r->esquerda->direita->cliente->id != 15) result = 0;
	if (r->direita->cliente->id != 25) result = 0;
	if (r->direita->direita->cliente->id != 30) result = 0;

	print_check(result, "Estrutura correta após rotação esquerda");

	return result;
}

int test_avl_structure_after_left_right_rotation()
{
	print_header("Testando estrutura AVL após rotação esquerda-direita");

	print_action("Inserindo clientes para forçar rotação esquerda-direita...");
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

	print_action("Verificando estrutura da árvore...");
	int result = 1;
	if (r->cliente->id != 13) result = 0;
	if (r->esquerda->cliente->id != 10) result = 0;
	if (r->esquerda->esquerda->cliente->id != 5) result = 0;
	if (r->esquerda->direita->cliente->id != 12) result = 0;
	if (r->direita->cliente->id != 15) result = 0;
	if (r->direita->direita->cliente->id != 20) result = 0;

	print_check(result, "Estrutura correta após rotação esquerda-direita");

	return result;
}

int test_avl_structure_after_right_left_rotation()
{
	print_header("Testando estrutura AVL após rotação direita-esquerda");

	print_action("Inserindo clientes para forçar rotação direita-esquerda...");
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

	print_action("Verificando estrutura da árvore...");
	int result = 1;
	if (r->cliente->id != 25) result = 0;
	if (r->esquerda->cliente->id != 15) result = 0;
	if (r->esquerda->esquerda->cliente->id != 10) result = 0;
	if (r->direita->cliente->id != 30) result = 0;
	if (r->direita->direita->cliente->id != 40) result = 0;
	if (r->direita->esquerda->cliente->id != 29) result = 0;

	print_check(result, "Estrutura correta após rotação direita-esquerda");

	return result;
}

int main()
{
	print_banner_principal("SUITE DE TESTES COMPLETOS: ESTRUTURA AVL");

	int result = 1;
	if (!test_avl_inserir()) result = 0;
	if (!test_avl_atualizar_nome()) result = 0;
	if (!test_avl_busca_vazia()) result = 0;
	if (!test_avl_structure_after_right_rotation()) result = 0;
	if (!test_avl_structure_after_left_rotation()) result = 0;
	if (!test_avl_structure_after_left_right_rotation()) result = 0;
	if (!test_avl_structure_after_right_left_rotation()) result = 0;

	print_footer(result, "TODOS OS TESTES AVL PASSARAM", "ALGUNS TESTES AVL FALHARAM");

	return !result;
}

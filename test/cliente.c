#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/cliente.h"
#include "test_utils.h"

int test_create_clients()
{
	print_header("Testando criação de clientes");

	print_action("Criando clientes A e B...");
	Cliente *c1 = criar_cliente("A");
	Cliente *c2 = criar_cliente("B");

	int result = 1;
	
	int c1_ok = (c1 != NULL && strcmp(c1->nome, "A") == 0 && c1->id == 1);
	print_check(c1_ok, "Cliente A criado corretamente (ID 1)");
	if (!c1_ok) result = 0;

	int c2_ok = (c2 != NULL && strcmp(c2->nome, "B") == 0 && c2->id == 2);
	print_check(c2_ok, "Cliente B criado corretamente (ID 2)");
	if (!c2_ok) result = 0;

	free(c1);
	free(c2);
	return result;
}

int main(void)
{
	print_banner_principal("SUITE DE TESTES COMPLETOS: ESTRUTURA CLIENTE");

	int passed = 1;
	passed &= test_create_clients();

	print_footer(passed, "TODOS OS TESTES CLIENTE PASSARAM", "ALGUNS TESTES CLIENTE FALHARAM");

	return !passed;
}

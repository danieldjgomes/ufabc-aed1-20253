#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/cliente.h"

	int test_create_clients()
{
	printf("Testando criação de clientes...\n");
	Cliente *c1 = criar_cliente("A");
	Cliente *c2 = criar_cliente("B");
	if (c1 == NULL || strcmp(c1->nome, "A") != 0 || c1->id != 1)
	{
		return 0;
	}
	if (c2 == NULL || strcmp(c2->nome, "B") != 0 || c2->id != 2)
	{
		return 0;
	}
	free(c1);
	free(c2);
	return 1;
}

int main(void)
{
	int passed = 1;

	passed &= test_create_clients();

	if (passed)
	{
		printf("Todos os testes passaram!\n");
		return 0;
	}
	else
	{
		printf("Alguns testes falharam.\n");
		return 1;
	}
}

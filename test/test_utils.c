#include <stdio.h>
#include "test_utils.h"

void print_banner_principal(const char *titulo)
{
	printf("\n" COR_AZUL "###########################################################" COR_RESET "\n");
	printf(COR_AZUL "#" COR_RESET COR_NEGRITO " %-53s " COR_RESET COR_AZUL "#" COR_RESET "\n", titulo);
	printf(COR_AZUL "###########################################################" COR_RESET "\n");
}

void print_header(const char *titulo)
{
	printf("\n" COR_CIANO "-----------------------------------------------------------" COR_RESET "\n");
	printf(COR_NEGRITO " %s " COR_RESET "\n", titulo);
	printf(COR_CIANO "-----------------------------------------------------------" COR_RESET "\n");
}

void print_action(const char *msg)
{
	printf("   " COR_AMARELO "-> " COR_RESET "%s\n", msg);
}

void print_info(const char *msg)
{
	printf("      " COR_CINZA "%s" COR_RESET "\n", msg);
}

void print_check(int passou, const char *msg)
{
	if (passou)
	{
		printf("   [" COR_VERDE "OK" COR_RESET "] %s\n", msg);
	}
	else
	{
		printf("   [" COR_VERMELHO "FALHA" COR_RESET "] %s\n", msg);
	}
}

void print_footer(int sucesso, const char *texto_sucesso, const char *texto_falha)
{
	printf("\n" COR_AZUL "###########################################################" COR_RESET "\n");
	if (sucesso)
	{
		printf(COR_AZUL "#" COR_RESET COR_NEGRITO " %-53s " COR_RESET COR_AZUL "#" COR_RESET "\n", texto_sucesso);
	}
	else
	{
		printf(COR_AZUL "#" COR_RESET COR_NEGRITO " %-53s " COR_RESET COR_AZUL "#" COR_RESET "\n", texto_falha);
	}
	printf(COR_AZUL "###########################################################" COR_RESET "\n");
}


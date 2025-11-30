#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "arvore_decisao.h"
#include "avl.h"
#include "categoria.h"
#include "supermercado.h"

int main(void)
{
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	// Data inicial do sistema
	int mes_atual_sistema = 11;
	int ano_atual_sistema = 2025;

	int opcao;

	do
	{
		printf("\n--- Sistema de Promocao Supermercado ---\n");
		printf("Data atual: %d/%d\n", mes_atual_sistema, ano_atual_sistema);
		printf("1. Inserir Cliente\n");
		printf("2. Buscar Cliente\n");
		printf("3. Listar Todos\n");
		printf("4. Classificar Cliente\n");
		printf("5. Registrar Compra\n");
		printf("6. Avançar Mês\n");
		printf("7. Sair\n");
		printf("Escolha: ");
		scanf("%d", &opcao);
		limpar_buffer();

		if (opcao == 1)
		{
			char nome[100];

			printf("Nome: ");
			fgets(nome, sizeof(nome), stdin);
			size_t len = strlen(nome);
			if (len > 0 && nome[len - 1] == '\n')
				nome[len - 1] = '\0';

			Cliente *c = criar_cliente(nome);
			avl = avl_inserir(avl, c, NULL);

			printf("\nCliente cadastrado com sucesso:\n");
			imprimir_cliente(c);
		}
		else if (opcao == 2)
		{
			int id;
			printf("ID do cliente: ");
			scanf("%d", &id);

			NoAVL *no = avl_buscar(avl, id);
			if (no)
			{
				imprimir_cliente(no->cliente);
			}
			else
			{
				printf("Cliente nao encontrado.\n");
			}
		}
		else if (opcao == 3)
		{
			printf("\n--- Lista de Clientes ---\n");
			avl_imprimir_em_ordem(avl);
		}
		else if (opcao == 4)
		{
			int id;
			printf("ID do cliente para classificar: ");
			scanf("%d", &id);

			NoAVL *no = avl_buscar(avl, id);
			if (no == NULL)
			{
				printf("Cliente nao encontrado.\n");
			}
			else
			{
				classificar_cliente_mensal(regras, no->cliente, mes_atual_sistema, ano_atual_sistema);
				printf("\nCliente classificado:\n");
				imprimir_cliente(no->cliente);
			}
		}
		else if (opcao == 5)
		{
			int id;
			float valor;

			printf("ID do Cliente: ");
			scanf("%d", &id);

			NoAVL *no = avl_buscar(avl, id);
			if (no == NULL)
			{
				printf("Cliente nao encontrado.\n");
			}
			else
			{
				printf("Valor da Compra: ");
				scanf("%f", &valor);

				// Registrar compra no mês atual
				no->cliente->consumo_mes_atual += valor;
				no->cliente->visitas_mes_atual++;

				printf("\nCompra registrada:\n");
				imprimir_cliente(no->cliente);
			}
		}
		else if (opcao == 6)
		{
			// Avançar mês: mover dados do mês atual para mês anterior e resetar
			printf("Avançando de %d/%d para ", mes_atual_sistema, ano_atual_sistema);

			// Atualizar todos os clientes
			avancar_mes_todos_clientes(avl, regras);

			// Avançar data do sistema
			mes_atual_sistema++;
			if (mes_atual_sistema > 12)
			{
				mes_atual_sistema = 1;
				ano_atual_sistema++;
			}

			printf("%d/%d\n", mes_atual_sistema, ano_atual_sistema);
			printf("Todos os clientes foram atualizados para o novo mês.\n");
		}

	} while (opcao != 7);

	avl_liberar(avl);
	liberar_arvore_decisao(regras);

	return 0;
}

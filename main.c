#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "arvore_decisao.h"
#include "avl.h"
#include "categoria.h"

NoDecisao *construir_arvore_decisao_supermercado();
void avancar_mes_cliente(Cliente *c, NoDecisao *regras);
void avancar_mes_todos_clientes(NoAVL *raiz, NoDecisao *regras);
void limpar_buffer();
void aplicar_regras_arvore(NoDecisao *d, Cliente *c);
Categoria classificar_cliente_mensal(NoDecisao *regras, Cliente *c, int mes, int ano);

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

void avancar_mes_cliente(Cliente *c, NoDecisao *regras)
{
	if (c != NULL)
	{
		Categoria categoria_anterior = c->categoria;

		// Mover dados do mês atual para mês anterior
		c->consumo_mes_anterior = c->consumo_mes_atual;
		c->visitas_mes_anterior = c->visitas_mes_atual;

		// Resetar dados do mês atual
		c->consumo_mes_atual = 0.0;
		c->visitas_mes_atual = 0;

		// Atualizar mês/ano do cliente
		c->mes_atual++;
		if (c->mes_atual > 12)
		{
			c->mes_atual = 1;
			c->ano_atual++;
		}

		// Classificar automaticamente baseado no mês anterior
		aplicar_regras_arvore(regras, c);

		// Verificar se houve mudança de categoria
		if (categoria_anterior != c->categoria)
		{
			printf("Cliente %d (%s): Mudou de '%s' para '%s'\n", c->id, c->nome,
				   nome_categoria(categoria_anterior), nome_categoria(c->categoria));
		}
	}
}

void avancar_mes_todos_clientes(NoAVL *raiz, NoDecisao *regras)
{
	if (raiz != NULL)
	{
		avancar_mes_todos_clientes(raiz->esquerda, regras);
		avancar_mes_cliente(raiz->cliente, regras);
		avancar_mes_todos_clientes(raiz->direita, regras);
	}
}

NoDecisao *construir_arvore_decisao_supermercado()
{
	/*
	 * Nova árvore baseada em visitas e gasto do mês anterior:
	 * Visitas mês anterior >= 2?
	 *   SIM: Gasto mês anterior >= 200?
	 *        SIM: Platina
	 *        NAO: Ouro
	 *   NAO: Gasto mês anterior >= 100?
	 *        SIM: Prata
	 *        NAO: Bronze
	 */

	// Folhas
	NoDecisao *f_platina = criar_folha_categoria(PLATINUM);
	NoDecisao *f_ouro = criar_folha_categoria(GOLD);
	NoDecisao *f_prata = criar_folha_categoria(SILVER);
	NoDecisao *f_bronze = criar_folha_categoria(BRONZE);

	// Ramos para visitas >= 2
	NoDecisao *consumo_alto_check = criar_no_decisao(TIPO_CONSUMO_MES_ANTERIOR, 200.0, f_ouro, f_platina);

	// Ramos para visitas < 2
	NoDecisao *consumo_baixo_check = criar_no_decisao(TIPO_CONSUMO_MES_ANTERIOR, 100.0, f_bronze, f_prata);

	// Raiz: verificação de visitas
	NoDecisao *raiz_check = criar_no_decisao(TIPO_VISITAS_MES_ANTERIOR, 2.0, consumo_baixo_check, consumo_alto_check);

	return raiz_check;
}

void limpar_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void aplicar_regras_arvore(NoDecisao *d, Cliente *c)
{
	if (d == NULL)
		return;

	if (d->tipo == TIPO_CATEGORIA)
	{
		c->categoria = d->categoria;
		return;
	}

	int condicao = 0;
	if (d->tipo == TIPO_VISITAS_MES_ANTERIOR)
	{
		condicao = (c->visitas_mes_anterior < d->valor_corte);
	}
	else if (d->tipo == TIPO_CONSUMO_MES_ANTERIOR)
	{
		condicao = (c->consumo_mes_anterior < d->valor_corte);
	}

	if (condicao)
	{
		aplicar_regras_arvore(d->esquerda, c);
	}
	else
	{
		aplicar_regras_arvore(d->direita, c);
	}
}

Categoria classificar_cliente_mensal(NoDecisao *regras, Cliente *c, int mes, int ano)
{
	(void)mes; // Suprimir warning de parâmetro não usado
	(void)ano; // Suprimir warning de parâmetro não usado

	if (regras == NULL || c == NULL)
		return STANDARD;

	aplicar_regras_arvore(regras, c);
	return c->categoria;
}

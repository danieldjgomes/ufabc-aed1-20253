#include <stdio.h>
#include <stdlib.h>
#include "supermercado.h"

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

int avancar_mes_cliente(Cliente *c, NoDecisao *regras)
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

		// Retorna 1 se houve mudança, 0 se não
		if (categoria_anterior != c->categoria)
		{
			printf("Cliente %d (%s): Mudou de '%s' para '%s'\n", c->id, c->nome,
				   nome_categoria(categoria_anterior), nome_categoria(c->categoria));
			return 1;
		}
	}
	return 0;
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

void limpar_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}


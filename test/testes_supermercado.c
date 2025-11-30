#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/cliente.h"
#include "../src/arvore_decisao.h"
#include "../src/avl.h"
#include "../src/categoria.h"
#include "../src/supermercado.h"
#include "test_utils.h"

// ============================================================================
// PARTE 1: TESTES FUNCIONAIS BÁSICOS
// ============================================================================

void teste_insercao_simples()
{
	print_header("TESTE 1: Inserção e Classificação Inicial");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Criando objeto cliente 'Thiago Dias'...");
	Cliente *c = criar_cliente("Thiago Dias");

	print_action("Inserindo na árvore AVL...");
	avl = avl_inserir(avl, c, regras);

	NoAVL *busca = avl_buscar(avl, c->id);
	print_check(busca != NULL, "Cliente inserido e encontrado na AVL");

	char buffer[100];
	sprintf(buffer, "Categoria definida: %s", nome_categoria(c->categoria));
	print_info(buffer);

	print_check(c->categoria == STANDARD, "Categoria inicial STANDARD atribuída corretamente");

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

void teste_classificacao_platina()
{
	print_header("TESTE 2: Evolução para PLATINA");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Inserindo 'Lucas Marin'...");
	Cliente *c = criar_cliente("Lucas Marin");
	avl = avl_inserir(avl, c, regras);

	print_action("Registrando compra de R$ 200.00...");
	avl_realizar_compra(avl, c->id, 200.0, 11, 2025, regras);

	print_action("Registrando compra de R$ 50.00...");
	avl_realizar_compra(avl, c->id, 50.0, 11, 2025, regras);

	print_info("Status Atual: Gasto R$ 250.00 | Visitas: 2");

	print_action("Virando o mês (acionando classificação)...");
	avancar_mes_todos_clientes(avl, regras);

	int sucesso = (c->categoria == PLATINUM);
	print_check(sucesso, "Cliente virou Platina corretamente");
	if (!sucesso)
		printf("      Recebido: %s\n", nome_categoria(c->categoria));

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

void teste_rebaixamento()
{
	print_header("TESTE 3: Rebaixamento de Categoria");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Inserindo 'Pedro DeGan'...");
	Cliente *c = criar_cliente("Pedro DeGan");
	avl = avl_inserir(avl, c, regras);

	print_action("Mês 1: Forçando status Platina (Gasto 300, 2 Visitas)...");
	avl_realizar_compra(avl, c->id, 300.0, 11, 2025, regras);
	avl_realizar_compra(avl, c->id, 10.0, 11, 2025, regras);
	avancar_mes_todos_clientes(avl, regras);
	print_info("Cliente agora deve ser Platina.");

	print_action("Mês 2: Cliente gasta R$ 1000.00 mas visita APENAS 1 vez...");
	avl_realizar_compra(avl, c->id, 1000.0, 12, 2025, regras);

	print_action("Virando o mês para recalcular...");
	avancar_mes_todos_clientes(avl, regras);

	// Na sua árvore: Visitas < 2 -> Consumo >= 100 -> SILVER (Prata)
	int sucesso = (c->categoria == SILVER);
	print_check(sucesso, "Cliente caiu de Platina para Prata (Frequência baixa)");
	if (!sucesso)
		printf("      Recebido: %s\n", nome_categoria(c->categoria));

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

// ============================================================================
// PARTE 2: TESTES DE ESTRESSE E ERROS
// ============================================================================

void teste_reembolso()
{
	print_header("TESTE 4: Estorno e Valores Negativos");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Inserindo 'Lucas Reis'...");
	Cliente *c = criar_cliente("Lucas Reis");
	avl = avl_inserir(avl, c, regras);

	print_action("Compra inicial: R$ 500.00");
	avl_realizar_compra(avl, c->id, 500.0, 11, 2025, regras);

	print_action("Visita extra (para cumprir requisito de freq)...");
	avl_realizar_compra(avl, c->id, 10.0, 11, 2025, regras);

	print_action("Aplicando ESTORNO de R$ -400.00...");
	avl_realizar_compra(avl, c->id, -400.0, 11, 2025, regras);

	print_info("Saldo Líquido Esperado: R$ 110.00");

	avancar_mes_todos_clientes(avl, regras);

	// Visitas >= 2, Gasto 110 ( < 200). Deve ser GOLD (Ouro)
	int sucesso = (c->categoria == GOLD);
	print_check(sucesso, "Cálculo de saldo líquido correto (Platina -> Ouro)");
	if (!sucesso)
		printf("      Recebido: %s\n", nome_categoria(c->categoria));

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

void teste_precisao_float()
{
	print_header("TESTE 5: Limite de Precisão (199.99 vs 200.00)");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Inserindo 'Daniel Gomes'...");
	Cliente *c = criar_cliente("Daniel Gomes");
	avl = avl_inserir(avl, c, regras);

	print_action("Registrando compra de R$ 199.99...");
	avl_realizar_compra(avl, c->id, 199.99, 11, 2025, regras);

	print_action("Adicionando visita extra...");
	avl_realizar_compra(avl, c->id, 0.0, 11, 2025, regras);

	print_action("Verificando se sistema arredonda errado...");
	avancar_mes_todos_clientes(avl, regras);

	// Visitas >= 2. Gasto 199.99 (< 200). Deve ser GOLD, NÃO PLATINUM.
	int sucesso = (c->categoria != PLATINUM);
	print_check(sucesso, "Rigor matemático: 199.99 não ativou regra de 200.00");
	printf("      Categoria Final: %s\n", nome_categoria(c->categoria));

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

void teste_ids_duplicados()
{
	print_header("TESTE 6: Integridade: IDs Duplicados");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Criando cliente 'Dona Maria 1' (ID X)...");
	Cliente *c1 = criar_cliente("Dona Maria 1");
	int id_alvo = c1->id;
	avl = avl_inserir(avl, c1, regras);

	print_action("Criando cliente 'Dona Maria 2'...");
	Cliente *c2 = criar_cliente("Dona Maria 2");

	print_action("Forçando ID do clone (Dona Maria 2) para ser igual ao Dona Maria 1...");
	c2->id = id_alvo;

	print_action("Tentando inserir o Clone na AVL...");
	avl = avl_inserir(avl, c2, regras);

	print_action("Buscando quem permaneceu na árvore...");
	NoAVL *busca = avl_buscar(avl, id_alvo);

	if (busca)
	{
		print_check(1, "Árvore manteve integridade estrutural após inserção duplicada");
	}

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

void teste_caos()
{
	print_header("TESTE 7: Teste de Valores Extremos");
	NoAVL *avl = avl_criar();
	NoDecisao *regras = construir_arvore_decisao_supermercado();

	print_action("Inserindo 'Algoritmo e Estrutura de Dados I'...");
	Cliente *c = criar_cliente("Algoritmo e Estrutura de Dados I");
	avl = avl_inserir(avl, c, regras);

	print_action("Somando compra de +1.000.000,00...");
	avl_realizar_compra(avl, c->id, 1000000.0, 11, 2025, regras);

	print_action("Subtraindo estorno de -2.000.000,00...");
	avl_realizar_compra(avl, c->id, -2000000.0, 11, 2025, regras);

	print_action("Somando compra de +1.000.250,00...");
	avl_realizar_compra(avl, c->id, 1000250.0, 11, 2025, regras);

	print_info("Saldo esperado: 250.00");

	// Forçar visitas para garantir Platina se saldo estiver ok
	c->visitas_mes_atual = 2;

	avancar_mes_todos_clientes(avl, regras);

	// Gasto 250, Visitas 2 -> Platina
	int sucesso = (c->categoria == PLATINUM && c->consumo_mes_anterior == 250.0);
	print_check(sucesso, "Sistema processou a soma de milhões corretamente");
	if (!sucesso)
		printf("      Saldo final: %.2f | Categoria: %s\n", c->consumo_mes_anterior, nome_categoria(c->categoria));

	avl_liberar(avl);
	liberar_arvore_decisao(regras);
}

int main()
{
	print_banner_principal("SUITE DE TESTES COMPLETOS: SISTEMA MERCADO");

	teste_insercao_simples();
	teste_classificacao_platina();
	teste_rebaixamento();

	printf("\n");

	teste_reembolso();
	teste_precisao_float();
	teste_ids_duplicados();
	teste_caos();

	print_footer(1, "TODOS OS TESTES FINALIZADOS", "FALHA NOS TESTES");

	return 0;
}

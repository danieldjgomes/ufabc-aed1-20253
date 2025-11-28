#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/cliente.h"
#include "../src/arvore_decisao.h"
#include "../src/avl.h"

// --- Cores e Formatação para Terminal ---
#define COR_RESET "\033[0m"
#define COR_VERDE "\033[32m"
#define COR_VERMELHO "\033[31m"
#define COR_AMARELO "\033[33m"
#define COR_AZUL "\033[34m"
#define COR_MAGENTA "\033[35m"
#define COR_NEGRITO "\033[1m"
#define COR_CIANO "\033[36m"
#define COR_CINZA "\033[90m"

// --- Funções de Ajuda Visual ---
void print_banner_principal() {
    printf("\n" COR_AZUL "###########################################################" COR_RESET "\n");
    printf(COR_AZUL "#" COR_RESET COR_NEGRITO "      SUITE DE TESTES COMPLETOS: SISTEMA MERCADO       " COR_RESET COR_AZUL "#" COR_RESET "\n");
    printf(COR_AZUL "###########################################################" COR_RESET "\n");
}

void print_header(const char* titulo) {
    printf("\n" COR_CIANO "-----------------------------------------------------------" COR_RESET "\n");
    printf(COR_NEGRITO " %s " COR_RESET "\n", titulo);
    printf(COR_CIANO "-----------------------------------------------------------" COR_RESET "\n");
}

void print_action(const char* msg) {
    printf("   " COR_AMARELO "-> " COR_RESET "%s\n", msg);
}

void print_info(const char* msg) {
    printf("      " COR_CINZA "%s" COR_RESET "\n", msg);
}

void print_check(int passou, const char* msg) {
    if (passou) {
        printf("   [" COR_VERDE "OK" COR_RESET "] %s\n", msg);
    } else {
        printf("   [" COR_VERMELHO "FALHA" COR_RESET "] %s\n", msg);
    }
}

// --- Setup das Regras de Negócio (Mock) ---
NoDecisao* setup_regras() {
    NoDecisao* f_platina = criar_folha_categoria("Platina");
    NoDecisao* f_ouro = criar_folha_categoria("Ouro");
    NoDecisao* f_prata = criar_folha_categoria("Prata");
    NoDecisao* f_bronze = criar_folha_categoria("Bronze");

    NoDecisao* consumo_alto = criar_no_decisao(TIPO_CONSUMO_MES_ANTERIOR, 200.0, f_ouro, f_platina);
    NoDecisao* consumo_baixo = criar_no_decisao(TIPO_CONSUMO_MES_ANTERIOR, 100.0, f_bronze, f_prata);
    NoDecisao* raiz = criar_no_decisao(TIPO_VISITAS_MES_ANTERIOR, 2.0, consumo_baixo, consumo_alto);
    
    return raiz;
}

// --- Simulador de Tempo (Virada de Mês) ---
void auxiliar_avancar_mes_todos(NoAVL* raiz, NoDecisao* regras) {
    if (raiz != NULL) {
        auxiliar_avancar_mes_todos(raiz->esquerda, regras);
        
        // Log extra para mostrar que está processando alguém
        printf("      [SISTEMA] Processando virada de mês para ID %d (%s)...\n", raiz->cliente->id, raiz->cliente->nome);

        raiz->cliente->consumo_mes_anterior = raiz->cliente->consumo_mes_atual;
        raiz->cliente->visitas_mes_anterior = raiz->cliente->visitas_mes_atual;
        raiz->cliente->consumo_mes_atual = 0.0;
        raiz->cliente->visitas_mes_atual = 0;
        
        raiz->cliente->mes_atual++;
        if (raiz->cliente->mes_atual > 12) {
            raiz->cliente->mes_atual = 1;
            raiz->cliente->ano_atual++;
        }
        
        classificar_cliente_mensal(regras, raiz->cliente, raiz->cliente->mes_atual, raiz->cliente->ano_atual);
        
        auxiliar_avancar_mes_todos(raiz->direita, regras);
    }
}

// ============================================================================
// PARTE 1: TESTES FUNCIONAIS BÁSICOS
// ============================================================================

void teste_insercao_simples() {
    print_header("TESTE 1: Inserção e Classificação Inicial");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();

    print_action("Criando objeto cliente 'Thiago Dias'...");
    Cliente* c = criar_cliente("Thiago Dias");
    
    print_action("Inserindo na árvore AVL...");
    avl = avl_inserir(avl, c, regras);

    NoAVL* busca = avl_buscar(avl, c->id);
    print_check(busca != NULL, "Cliente inserido e encontrado na AVL");
    
    char buffer[100];
    sprintf(buffer, "Categoria definida: %s", c->categoria);
    print_info(buffer);
    
    print_check(strcmp(c->categoria, "Bronze") == 0 || strcmp(c->categoria, "Indefinido") == 0, 
                "Categoria inicial atribuída automaticamente");

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

void teste_classificacao_platina() {
    print_header("TESTE 2: Evolução para PLATINA");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();
    
    print_action("Inserindo 'Lucas Marin'...");
    Cliente* c = criar_cliente("Lucas Marin");
    avl = avl_inserir(avl, c, regras);

    print_action("Registrando compra de R$ 200.00...");
    avl_realizar_compra(avl, c->id, 200.0, 11, 2025, regras);
    
    print_action("Registrando compra de R$ 50.00...");
    avl_realizar_compra(avl, c->id, 50.0, 11, 2025, regras);
    
    print_info("Status Atual: Gasto R$ 250.00 | Visitas: 2");
    
    print_action("Virando o mês (acionando classificação)...");
    auxiliar_avancar_mes_todos(avl, regras); 

    int sucesso = (strcmp(c->categoria, "Platina") == 0);
    print_check(sucesso, "Cliente virou Platina corretamente");
    if(!sucesso) printf("      Recebido: %s\n", c->categoria);

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

void teste_rebaixamento() {
    print_header("TESTE 3: Rebaixamento de Categoria");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();
    
    print_action("Inserindo 'Pedro DeGan'...");
    Cliente* c = criar_cliente("Pedro DeGan");
    avl = avl_inserir(avl, c, regras);

    print_action("Mês 1: Forçando status Platina (Gasto 300, 2 Visitas)...");
    avl_realizar_compra(avl, c->id, 300.0, 11, 2025, regras);
    avl_realizar_compra(avl, c->id, 10.0, 11, 2025, regras); 
    auxiliar_avancar_mes_todos(avl, regras);
    print_info("Cliente agora é Platina.");

    print_action("Mês 2: Cliente gasta R$ 1000.00 mas visita APENAS 1 vez...");
    avl_realizar_compra(avl, c->id, 1000.0, 12, 2025, regras);
    
    print_action("Virando o mês para recalcular...");
    auxiliar_avancar_mes_todos(avl, regras);

    int sucesso = (strcmp(c->categoria, "Prata") == 0);
    print_check(sucesso, "Cliente caiu de Platina para Prata (Frequência baixa)");

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

// ============================================================================
// PARTE 2: TESTES DE ESTRESSE E ERROS
// ============================================================================

void teste_reembolso() {
    print_header("TESTE 4: Estorno e Valores Negativos");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();
    
    print_action("Inserindo 'Lucas Reis'...");
    Cliente* c = criar_cliente("Lucas Reis");
    avl = avl_inserir(avl, c, regras);

    print_action("Compra inicial: R$ 500.00");
    avl_realizar_compra(avl, c->id, 500.0, 11, 2025, regras);
    
    print_action("Visita extra (para cumprir requisito de freq)...");
    avl_realizar_compra(avl, c->id, 10.0, 11, 2025, regras);

    print_action("Aplicando ESTORNO de R$ -400.00...");
    avl_realizar_compra(avl, c->id, -400.0, 11, 2025, regras);
    
    print_info("Saldo Líquido Esperado: R$ 110.00");

    auxiliar_avancar_mes_todos(avl, regras);

    int sucesso = (strcmp(c->categoria, "Ouro") == 0);
    print_check(sucesso, "Cálculo de saldo líquido correto (Platina -> Ouro)");

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

void teste_precisao_float() {
    print_header("TESTE 5: Limite de Precisão (199.99 vs 200.00)");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();
    
    print_action("Inserindo 'Daniel Gomes'...");
    Cliente* c = criar_cliente("Daniel Gomes");
    avl = avl_inserir(avl, c, regras);

    print_action("Registrando compra de R$ 199.99...");
    avl_realizar_compra(avl, c->id, 199.99, 11, 2025, regras);
    
    print_action("Adicionando visita extra...");
    avl_realizar_compra(avl, c->id, 0.0, 11, 2025, regras); 

    print_action("Verificando se sistema arredonda errado...");
    auxiliar_avancar_mes_todos(avl, regras);

    int sucesso = (strcmp(c->categoria, "Platina") != 0);
    print_check(sucesso, "Rigor matemático: 199.99 não ativou regra de 200.00");
    printf("      Categoria Final: %s\n", c->categoria);

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

void teste_ids_duplicados() {
    print_header("TESTE 6: Integridade: IDs Duplicados");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();

    print_action("Criando cliente 'Dona Maria 1' (ID X)...");
    Cliente* c1 = criar_cliente("Dona Maria 1");
    int id_alvo = c1->id;
    avl = avl_inserir(avl, c1, regras);

    print_action("Criando cliente 'Dona Maria 2'...");
    Cliente* c2 = criar_cliente("Dona Maria 2");
    
    print_action("Forçando ID do clone (Dona Maria 2) para ser igual ao Dona Maria 1...");
    c2->id = id_alvo; 

    print_action("Tentando inserir o Clone na AVL...");
    avl = avl_inserir(avl, c2, regras);

    print_action("Buscando quem permaneceu na árvore...");
    NoAVL* busca = avl_buscar(avl, id_alvo);
    
    if (busca && strcmp(busca->cliente->nome, "Dona Maria 1") == 0) {
        print_check(1, "AVL protegeu contra duplicidade (Dona Maria 1 mantido)");
        free(c2); 
    } else {
        print_check(0, "FALHA: O clone sobrescreveu o Dona Maria 1!");
    }

    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

void teste_caos() {
    print_header("TESTE 7: Teste de Valores Extremos");
    NoAVL* avl = avl_criar();
    NoDecisao* regras = setup_regras();
    
    print_action("Inserindo 'Algoritmo e Estrutura de Dados I'...");
    Cliente* c = criar_cliente("Algoritmo e Estrutura de Dados I");
    avl = avl_inserir(avl, c, regras);

    print_action("Somando compra de +1.000.000,00...");
    avl_realizar_compra(avl, c->id, 1000000.0, 11, 2025, regras);
    
    print_action("Subtraindo estorno de -2.000.000,00...");
    avl_realizar_compra(avl, c->id, -2000000.0, 11, 2025, regras);
    
    print_action("Somando compra de +1.000.250,00...");
    avl_realizar_compra(avl, c->id, 1000250.0, 11, 2025, regras);

    print_info("Saldo esperado: 250.00");
    
    auxiliar_avancar_mes_todos(avl, regras);

    int sucesso = (strcmp(c->categoria, "Platina") == 0 && c->consumo_mes_anterior == 250.0);
    print_check(sucesso, "Sistema processou a soma de milhões corretamente");
    
    avl_liberar(avl);
    liberar_arvore_decisao(regras);
}

// --- Main do Teste Completo ---
int main() {
    print_banner_principal();

    teste_insercao_simples();
    teste_classificacao_platina();
    teste_rebaixamento();

    printf("\n");
    
    teste_reembolso();
    teste_precisao_float();
    teste_ids_duplicados();
    teste_caos();

    printf("\n" COR_AZUL "###########################################################" COR_RESET "\n");
    printf(COR_AZUL "#" COR_RESET COR_NEGRITO "            TODOS OS TESTES FINALIZADOS                " COR_RESET COR_AZUL "#" COR_RESET "\n");
    printf(COR_AZUL "###########################################################" COR_RESET "\n");

    return 0;
}
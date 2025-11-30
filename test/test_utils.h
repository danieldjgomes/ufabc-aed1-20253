#ifndef TEST_UTILS_H
#define TEST_UTILS_H

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

/**
 * Imprime o banner principal do teste.
 * @param titulo O título que será exibido dentro do banner.
 */
void print_banner_principal(const char *titulo);

/**
 * Imprime um cabeçalho para uma seção de teste.
 * @param titulo O título da seção.
 */
void print_header(const char *titulo);

/**
 * Imprime uma ação sendo realizada (ex: "Inserindo cliente...").
 * @param msg A descrição da ação.
 */
void print_action(const char *msg);

/**
 * Imprime uma informação adicional (ex: "Saldo esperado: 100").
 * @param msg A informação.
 */
void print_info(const char *msg);

/**
 * Verifica uma condição e imprime [OK] ou [FALHA].
 * @param passou 1 se o teste passou, 0 caso contrário.
 * @param msg A descrição do que foi testado.
 */
void print_check(int passou, const char *msg);

/**
 * Imprime o banner de finalização dos testes.
 * @param sucesso 1 se todos os testes passaram, 0 caso contrário.
 * @param texto_sucesso Texto a ser exibido em caso de sucesso.
 * @param texto_falha Texto a ser exibido em caso de falha.
 */
void print_footer(int sucesso, const char *texto_sucesso, const char *texto_falha);

#endif


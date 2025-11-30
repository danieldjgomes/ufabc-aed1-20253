CC = gcc
CFLAGS = -Wall -Wextra -I ./src

# Nomes dos executáveis
EXEC_APP = sistema_mercado
EXEC_SYS_TESTS = sys_tests
EXEC_AVL_TESTS = avl_tests
EXEC_CLIENT_TESTS = client_tests

MAIN_SRC = src/main.c
SYS_TEST_SRC = test/testes_supermercado.c
AVL_TEST_SRC = test/avl.c
CLIENT_TEST_SRC = test/cliente.c

COMMON_SRC = src/avl.c src/arvore_decisao.c src/cliente.c src/categoria.c

# --- Regras Principais ---

all: app sys_tests avl_tests client_tests

app: $(MAIN_SRC) $(COMMON_SRC)
	@echo "Compilando Sistema Principal..."
	$(CC) $(CFLAGS) $(MAIN_SRC) $(COMMON_SRC) -o $(EXEC_APP)
	@echo "Sucesso! Execute com: ./$(EXEC_APP)"

sys_tests: $(SYS_TEST_SRC) $(COMMON_SRC)
	@echo "Compilando Suite de Testes do Sistema..."
	$(CC) $(CFLAGS) $(SYS_TEST_SRC) $(COMMON_SRC) -o $(EXEC_SYS_TESTS)
	@echo "Sucesso! Execute com: ./$(EXEC_SYS_TESTS)"

avl_tests: $(AVL_TEST_SRC) $(COMMON_SRC)
	@echo "Compilando Testes da estrutura AVL..."
	$(CC) $(CFLAGS) $(AVL_TEST_SRC) $(COMMON_SRC) -o $(EXEC_AVL_TESTS)
	@echo "Sucesso! Execute com: ./$(EXEC_AVL_TESTS)"

client_tests: $(CLIENT_TEST_SRC) $(COMMON_SRC)
	@echo "Compilando Testes da estrutura Cliente..."
	$(CC) $(CFLAGS) $(CLIENT_TEST_SRC) $(COMMON_SRC) -o $(EXEC_CLIENT_TESTS)
	@echo "Sucesso! Execute com: ./$(EXEC_CLIENT_TESTS)"

run: app
	@echo "--- Executando Sistema ---"
	@./$(EXEC_APP)

run_sys_tests: sys_tests
	@echo "--- Executando Testes ---"
	@./$(EXEC_SYS_TESTS)

run_avl_tests: avl_tests
	@echo "--- Executando Testes da estrutura AVL ---"
	@./$(EXEC_AVL_TESTS)

run_client_tests: client_tests
	@echo "--- Executando Testes da estrutura Cliente ---"
	@./$(EXEC_CLIENT_TESTS)

clean:
	@echo "Limpando executáveis..."
	@rm -f $(EXEC_APP) $(EXEC_SYS_TESTS) $(EXEC_AVL_TESTS) $(EXEC_CLIENT_TESTS) $(EXEC_APP).exe $(EXEC_SYS_TESTS).exe $(EXEC_AVL_TESTS).exe $(EXEC_CLIENT_TESTS).exe
	@echo "Limpeza concluída."

.PHONY: all app sys_tests avl_tests run run_sys_tests run_avl_tests run_client_tests clean

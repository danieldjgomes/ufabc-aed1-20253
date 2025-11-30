CC = gcc
CFLAGS = -Wall -Wextra -I./src

# Nomes dos executáveis
EXEC_APP = sistema_mercado
EXEC_TEST = suite_testes

COMMON_SRC = src/avl.c src/arvore_decisao.c src/cliente.c src/categoria.c

MAIN_SRC = src/main.c

TEST_SRC = test/testes_supermercado.c

# --- Regras Principais ---

all: app tests

app: $(MAIN_SRC) $(COMMON_SRC)
	@echo "Compilando Sistema Principal..."
	$(CC) $(CFLAGS) $(MAIN_SRC) $(COMMON_SRC) -o $(EXEC_APP)
	@echo "Sucesso! Execute com: ./$(EXEC_APP)"

tests: $(TEST_SRC) $(COMMON_SRC)
	@echo "Compilando Suite de Testes..."
	$(CC) $(CFLAGS) $(TEST_SRC) $(COMMON_SRC) -o $(EXEC_TEST)
	@echo "Sucesso! Execute com: ./$(EXEC_TEST)"

run: app
	@echo "--- Executando Sistema ---"
	@./$(EXEC_APP)

run_tests: tests
	@echo "--- Executando Testes ---"
	@./$(EXEC_TEST)

clean:
	@echo "Limpando executáveis..."
	@rm -f $(EXEC_APP) $(EXEC_TEST) $(EXEC_APP).exe $(EXEC_TEST).exe
	@echo "Limpeza concluída."

.PHONY: all app tests run run_tests clean
build:
	gcc \
		-std=c11 \
		-o main avl.c cliente.c arvore_decisao.c main.c \
		-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wsign-conversion \

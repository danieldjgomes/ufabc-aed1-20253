build:
	gcc \
		-std=c11 \
		-o main src/*.c \
		-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wsign-conversion

test_avl:
	gcc \
		-std=c11 \
		-o test_avl src/avl.c src/cliente.c src/categoria.c test/avl.c;
	./test_avl;
	rm test_avl;

build:
	gcc \
		-std=c11 \
		-o main src/*.c \
		-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wsign-conversion \

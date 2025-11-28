#!/bin/bash

# Para parar se algum comando falhar
set -e

echo "Compilando main..."
gcc -o programa main.c avl.c cliente.c arvore_decisao.c

echo "Compilando testes_supermercado..."
gcc -o testes_supermercado testes_supermercado.c avl.c cliente.c arvore_decisao.c

echo "Compilação feita"
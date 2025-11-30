#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H

#include "cliente.h"
#include "arvore_decisao.h"
#include "avl.h"
#include "categoria.h"

NoDecisao *construir_arvore_decisao_supermercado();

void aplicar_regras_arvore(NoDecisao *d, Cliente *c);

Categoria classificar_cliente_mensal(NoDecisao *regras, Cliente *c, int mes, int ano);

int avancar_mes_cliente(Cliente *c, NoDecisao *regras);

void avancar_mes_todos_clientes(NoAVL *raiz, NoDecisao *regras);

void limpar_buffer();

#endif


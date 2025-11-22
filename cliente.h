#ifndef CLIENTE_H
#define CLIENTE_H

#include "categoria.h"

typedef struct cliente Cliente;

struct cliente
{
	int id;
	char nome[100];
	Categoria categoria;
	int mes_atual;
	int ano_atual;
	float consumo_mes_atual;
	int visitas_mes_atual;
	float consumo_mes_anterior;
	int visitas_mes_anterior;
};

Cliente *criar_cliente(const char *nome);

void destruir_cliente(Cliente *c);

void imprimir_cliente(Cliente *c);

#endif

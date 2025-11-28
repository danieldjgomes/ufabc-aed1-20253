#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

static int proximo_id = 1;

Cliente* criar_cliente(const char* nome) {
    Cliente* c = (Cliente*) malloc(sizeof(Cliente));
    if (c != NULL) {
        c->id = proximo_id++;
        strncpy(c->nome, nome, sizeof(c->nome) - 1);
        c->nome[sizeof(c->nome) - 1] = '\0';
        strcpy(c->categoria, "Indefinido");
        c->mes_atual = 11;
        c->ano_atual = 2025;
        c->consumo_mes_atual = 0.0;
        c->visitas_mes_atual = 0;
        c->consumo_mes_anterior = 0.0;
        c->visitas_mes_anterior = 0;
    }
    return c;
}

void destruir_cliente(Cliente* c) {
    if (c != NULL) {
        free(c);
    }
}

void imprimir_cliente(Cliente* c) {
    if (c != NULL) {
        float consumo_total = c->consumo_mes_atual + c->consumo_mes_anterior;
        int freq_total = c->visitas_mes_atual + c->visitas_mes_anterior;
        printf("ID: %d | Nome: %s | Consumo Total: %.2f | Freq: %d | Categoria: %s\n",
               c->id, c->nome, consumo_total, freq_total, c->categoria);
        printf("  Mês Atual (%d/%d): Consumo: %.2f | Visitas: %d\n",
               c->mes_atual, c->ano_atual, c->consumo_mes_atual, c->visitas_mes_atual);
        printf("  Mês Anterior: Consumo: %.2f | Visitas: %d\n",
               c->consumo_mes_anterior, c->visitas_mes_anterior);
    }
}

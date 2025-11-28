#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int id;
    char nome[100];
    char categoria[50];
    int mes_atual;
    int ano_atual;
    float consumo_mes_atual;
    int visitas_mes_atual;
    float consumo_mes_anterior;
    int visitas_mes_anterior;
} Cliente;

Cliente* criar_cliente(const char* nome);
void destruir_cliente(Cliente* c);
void imprimir_cliente(Cliente* c);

#endif

#ifndef CATEGORIA_H
#define CATEGORIA_H

typedef enum
{
	STANDARD,
	BRONZE,
	SILVER,
	GOLD,
	PLATINUM
} Categoria;

char *nome_categoria(Categoria);

// Helper to get the previous category
Categoria get_categoria_anterior(Categoria);

#endif

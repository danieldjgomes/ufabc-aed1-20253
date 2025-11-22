#include "categoria.h"

char *nome_categoria(Categoria c)
{
	switch (c)
	{
	case STANDARD:
		return "STANDARD";
	case BRONZE:
		return "BRONZE";
	case SILVER:
		return "SILVER";
	case GOLD:
		return "GOLD";
	case PLATINUM:
		return "PLATINUM";
	default:
		return "UNKNOWN";
	}
}

Categoria get_categoria_anterior(Categoria categoria)
{
	if (categoria == STANDARD)
	{
		return STANDARD;
	}
	return (Categoria)categoria - 1;
}

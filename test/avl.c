#include <stdio.h>
#include <stdlib.h>

#include "../src/avl.h"

int main()
{
	NoAVL *raiz = avl_criar();

	Cliente *c1 = criar_cliente("A");
	Cliente *c2 = criar_cliente("B");
	Cliente *c3 = criar_cliente("C");

	avl_inserir(&raiz, c1, NULL);
	avl_inserir(&raiz, c2, NULL);
	avl_inserir(&raiz, c3, NULL);

	avl_imprimir(raiz);

	avl_libera(raiz);
	return 0;
}

#ifndef IMAGEM_H
#define IMAGEM_H

#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>

typedef struct Imagem {
	unsigned char ***m;
	int w;
	int h;
	int numCanais;
} Imagem;

typedef struct Ladrilho {
	unsigned char cor[3];
	int i;
	int j;
	int tamanho;
} Ladrilho;


Imagem original, resultado;
Imagem alocarImagem(Imagem referencia);
void desalocarImagem(Imagem referencia);

#endif
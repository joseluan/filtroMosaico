#ifndef MEUFILTRO_H
#define MEUFILTRO_H

#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <time.h>
#include "imagem.h"

//declare aqui os seus widgets
GtkWidget *widgetControleNivel;
GtkWidget *widgetControleAngulo;
GtkWidget *widgetMisturarCanais;
GtkWidget *widgetAnguloEscolha;
GtkWidget *widgetAnguloAleatorio;
GtkWidget *widgetAnguloGradiente;
GtkWidget *widgetOrdemLadrilhos;
GtkWidget *widgetColorChooserDialog;
GtkWidget *label3, *label4;
GtkWidget *widgetCorFundo;
GtkWidget *container_filtro;
GtkWidget *aba1, *aba2, *aba3;
GtkWidget *frameDiversos, *frameCorFundo, *frameAngulacao;

Imagem meuFiltro(Imagem origem);
void inicializarWidgetsMeuFiltro();
void adicionarWidgetsMeuFiltro(GtkWidget *container);

//esta funcao esta implementada em filtroGtk.c, nao precisa alterar
void funcaoAplicar(GtkWidget *widget, gpointer data);

#endif



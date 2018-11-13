#include "meufiltro.h"

Imagem pintarLadrilho(Imagem tela, Ladrilho l){
	//preenchendo o ladrilho com a cor do meio
	for (int linha = l.i;linha <= (l.i + l.tamanho); ++linha){
		for (int coluna = l.j; coluna <= (l.j + l.tamanho); ++coluna){
			if(linha < tela.h && coluna < tela.w){
				tela.m[linha][coluna][0] = l.cor[0];
				tela.m[linha][coluna][1] = l.cor[1];
				tela.m[linha][coluna][2] = l.cor[2];
			}
		}	
	}
	return tela;
}

void inicializarWidgetsMeuFiltro() {
	//widgets das opcoes de filtro
	widgetControleNivel = 	gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 100, 3);
	widgetMisturarCanais = gtk_check_button_new_with_label("Misturar canais");
	widgetOrdemLadrilhos = gtk_check_button_new_with_label("Ordem dos ladrilhos aleatória");
	label3 = gtk_label_new("Tamanho do ladrilho");;
	g_signal_connect(G_OBJECT(widgetControleNivel), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
}

void adicionarWidgetsMeuFiltro(GtkWidget *container) {

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *hbtnbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
	gtk_container_add(GTK_CONTAINER(container), vbox);
	gtk_container_add(GTK_CONTAINER(vbox), label3);
	gtk_container_add(GTK_CONTAINER(vbox), widgetControleNivel);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetMisturarCanais);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetOrdemLadrilhos);
	gtk_container_add(GTK_CONTAINER(vbox), hbtnbox);
}

Imagem meuFiltro(Imagem origem) {
	int i, j;
	int nivel = (int) gtk_range_get_value(GTK_RANGE(widgetControleNivel));
	int isAleatorio = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetOrdemLadrilhos));
	
	Imagem destino = alocarImagem(origem);

	int ch1, ch2, ch3;

	ch1 = 0;
	ch2 = 1;
	ch3 = 2;

	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetMisturarCanais))) {
		ch1 = rand()%3;
		ch2 = (ch1+1+rand()%2)%3;
		ch3 = 3 - ch2 - ch1;
	}

	int qtdLadrilho_x = 0, qtdLadrilho_y = 0, qtdLadrilho;
	qtdLadrilho_x = (int) destino.h/nivel;
	qtdLadrilho_y = (int) destino.w/nivel;

	int qtdLadrilhoTotal = qtdLadrilho_y*qtdLadrilho_x, cont = 0;
	Ladrilho *ladrilhos = calloc(qtdLadrilhoTotal, sizeof(Ladrilho));

	//percorrendo ladrilhos
	for(i = 0; i < qtdLadrilho_x; ++i) {
		for(j = 0; j < qtdLadrilho_y; ++j) {
			int ii = i*(nivel), jj = j*(nivel);

			int pixel_x = (int) (ii+((nivel)/2));
			int pixel_y = (int) (jj+((nivel)/2));

			if(cont < qtdLadrilhoTotal){
				Ladrilho ldlh;
				ldlh.cor[0] = origem.m[pixel_x][pixel_y][ch1];
				ldlh.cor[1] = origem.m[pixel_x][pixel_y][ch2];
				ldlh.cor[2] = origem.m[pixel_x][pixel_y][ch3];
				ldlh.i = ii;
				ldlh.j = jj;
				ldlh.tamanho = nivel;

				ladrilhos[cont] = ldlh;
				cont++;
			}else{
				break;
			}
		}
	}
	//Pintando o ladrilho
	for (int i = 0; i < qtdLadrilhoTotal; ++i){
		if (isAleatorio){
			ladrilhos[i].i = rand()%(destino.h-1);
			ladrilhos[i].j = rand()%(destino.h-1);
		}
		
		pintarLadrilho(destino, ladrilhos[i]);				
	}

	free(ladrilhos);
	return destino;
}
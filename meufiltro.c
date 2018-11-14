#include "meufiltro.h"
#include <math.h>

int girarVetor(char tipo,Imagem img, int x, int y, int angulo){
	int new_angulo = (double) angulo;
	if(tipo == 'X' || tipo == 'x'){
		return ceil((cos(new_angulo)*(x-ceil(img.w/2)))+(sin(new_angulo)*(y-ceil(img.h/2)))) + ceil(img.w/2);
	}else if(tipo == 'Y' || tipo == 'y'){
		return ceil(((-1)*sin(new_angulo)*(x-ceil(img.w/2)))+(cos(new_angulo)*(y-ceil(img.h/2)))) + ceil(img.w/2);
	}
}

Imagem pintarLadrilho(Imagem tela, Ladrilho l, int angulo){
	//preenchendo o ladrilho com a cor do meio
	for (int linha = l.i;linha <= (l.i + l.tamanho); ++linha){
		for (int coluna = l.j; coluna <= (l.j + l.tamanho); ++coluna){
			int nlinha = linha, ncoluna = coluna;
			//if(angulo != 0){
				nlinha = girarVetor('x', tela, linha, coluna, angulo);
				ncoluna = girarVetor('y', tela, linha, coluna, angulo);
			//}

			if((nlinha < tela.h && nlinha >= 0) && (ncoluna < tela.w && ncoluna >= 0)){
				tela.m[nlinha][ncoluna][0] = l.cor[0];
				tela.m[nlinha][ncoluna][1] = l.cor[1];
				tela.m[nlinha][ncoluna][2] = l.cor[2];
			}
		}	
	}
	return tela;
}

void inicializarWidgetsMeuFiltro() {
	//widgets das opcoes de filtro
	widgetControleNivel = 	gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 100, 3);
	widgetControleAngulo = 	gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 360, 1);
	widgetMisturarCanais = gtk_check_button_new_with_label("Misturar canais");
	widgetOrdemLadrilhos = gtk_check_button_new_with_label("Ordem dos ladrilhos aleatória");
	label3 = gtk_label_new("Tamanho do ladrilho");
	label4 = gtk_label_new("Ângulo de rotação do ladrilho");
	g_signal_connect(G_OBJECT(widgetControleAngulo), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetControleNivel), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
}

void adicionarWidgetsMeuFiltro(GtkWidget *container) {

	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *hbtnbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
	gtk_container_add(GTK_CONTAINER(container), vbox);
	gtk_container_add(GTK_CONTAINER(vbox), label3);
	gtk_container_add(GTK_CONTAINER(vbox), widgetControleNivel);
	gtk_container_add(GTK_CONTAINER(vbox), label4);
	gtk_container_add(GTK_CONTAINER(vbox), widgetControleAngulo);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetMisturarCanais);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetOrdemLadrilhos);
	gtk_container_add(GTK_CONTAINER(vbox), hbtnbox);
}

Imagem meuFiltro(Imagem origem) {
	int i, j;
	int nivel = (int) gtk_range_get_value(GTK_RANGE(widgetControleNivel));
	int angulo = (int) gtk_range_get_value(GTK_RANGE(widgetControleAngulo));
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
		
		pintarLadrilho(destino, ladrilhos[i], angulo);				
	}

	free(ladrilhos);
	return destino;
}
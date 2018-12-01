#include "meufiltro.h"
#include <math.h>

int girarVetor(char tipo, Ladrilho l, int x, int y, double angulo){
	angulo /= 58;
	int meio_tamanho = (int) l.tamanho/2;
	int dx = l.i + meio_tamanho, dy = l.j + meio_tamanho;

	//translação 1
	int t1x = x - dx, t1y = y - dy;
	//rotação
	int rot_tx = ceil(t1x*cos(angulo) + t1y*sin(angulo));
	int rot_ty = ceil(t1y*cos(angulo) - t1x*sin(angulo)) ;
	//translação 2
	int t2x = rot_tx + dx;
	int t2y = rot_ty + dy;
	if(tipo == 'X' || tipo == 'x'){
		return t2x;
	}else if(tipo == 'Y' || tipo == 'y'){
		return t2y;
	}
}

Imagem pintarLadrilho(Imagem tela, Ladrilho l, double angulo){
	//preenchendo o ladrilho com a cor do meio
	for (int linha = l.i;linha <= (l.i + l.tamanho); ++linha){
		for (int coluna = l.j; coluna <= (l.j + l.tamanho); ++coluna){
			int nlinha = linha, ncoluna = coluna;
			nlinha = girarVetor('x', l, linha, coluna, angulo);
			ncoluna = girarVetor('y', l, linha, coluna, angulo);

			if((nlinha < tela.h && nlinha >= 0) && (ncoluna < tela.w && ncoluna >= 0)){
				tela.m[nlinha][ncoluna][0] = l.cor[0];
				tela.m[nlinha][ncoluna][1] = l.cor[1];
				tela.m[nlinha][ncoluna][2] = l.cor[2];
			}
		}	
	}
	return tela;
}

Imagem pintarLadrilhoGradiente(Imagem tela, Imagem origem, Ladrilho l){
	//preenchendo o ladrilho com a cor do meio
	int meiox_l = l.j + ceil(l.tamanho/2), meioy_l = l.i + ceil(l.tamanho/2);
	if((meioy_l+1 < tela.h && meioy_l-1 >= 0) &&
	   (meiox_l+1 < tela.w && meiox_l-1 >= 0)){
		int gradiente_x =  0;
		int gradiente_y = 0;
		
		//gradiente X
		for (int i = 0; i < 3; ++i){
			gradiente_x += (int) origem.m[meiox_l+1][meioy_l][i] + origem.m[meiox_l+1][meioy_l+1][i] + origem.m[meiox_l+1][meioy_l-1][i];
			gradiente_x -= (int) origem.m[meiox_l-1][meioy_l][i] + origem.m[meiox_l-1][meioy_l-1][i] + origem.m[meiox_l-1][meioy_l+1][i];
		}

		gradiente_x = ceil(gradiente_x/3);

		//gradiente Y
		for (int i = 0; i < 3; ++i){
			gradiente_y += (int) origem.m[meiox_l][meioy_l+1][i] + origem.m[meiox_l+1][meioy_l+1][i] + origem.m[meiox_l-1][meioy_l+1][i];
			gradiente_y -= (int) origem.m[meiox_l][meioy_l-1][i] + origem.m[meiox_l-1][meioy_l-1][i] + origem.m[meiox_l+1][meioy_l-1][i];
		}

		gradiente_y = ceil(gradiente_y/3);
		
		double angulo = atan2(gradiente_y*1.0, gradiente_x*1.0);
		tela = pintarLadrilho(tela, l, angulo);
	}
	return tela;
}

void inicializarWidgetsMeuFiltro() {
	//widgets das opcoes de filtro
	widgetControleNivel = 	gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 1, 100, 1);
	widgetControleAngulo = 	gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 90, 1);
	widgetMisturarCanais = gtk_check_button_new_with_label("Misturar canais");
	widgetAnguloEscolha = gtk_radio_button_new_with_label(NULL,"Escolher Angulação");
	widgetAnguloGradiente = gtk_radio_button_new_with_label(gtk_radio_button_get_group (GTK_RADIO_BUTTON(widgetAnguloEscolha)),
															"Angulação pelo gradiente");
	widgetAnguloAleatorio = gtk_radio_button_new_with_label(gtk_radio_button_get_group (GTK_RADIO_BUTTON(widgetAnguloEscolha)),
															"Angulação dos ladrilhos aleatória");
	widgetOrdemLadrilhos = gtk_check_button_new_with_label("Ordem dos ladrilhos aleatória");

	label3 = gtk_label_new("Tamanho do ladrilho");
	label4 = gtk_label_new("Ângulo de rotação do ladrilho");
	
	g_signal_connect(G_OBJECT(widgetControleAngulo), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetControleNivel), "value-changed", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetAnguloAleatorio), "clicked", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetAnguloEscolha), "clicked", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetAnguloGradiente), "clicked", G_CALLBACK(funcaoAplicar), NULL);
	g_signal_connect(G_OBJECT(widgetOrdemLadrilhos), "clicked", G_CALLBACK(funcaoAplicar), NULL);
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
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetAnguloEscolha);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetAnguloAleatorio);
	gtk_container_add(GTK_CONTAINER(hbtnbox), widgetAnguloGradiente);
	gtk_container_add(GTK_CONTAINER(vbox), hbtnbox);
}

Imagem meuFiltro(Imagem origem) {
	int i, j;
	int nivel = (int) gtk_range_get_value(GTK_RANGE(widgetControleNivel));
	double angulo = gtk_range_get_value(GTK_RANGE(widgetControleAngulo));
	int isAleatorio = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetOrdemLadrilhos));
	int isAnguloAleatorio = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetAnguloAleatorio));
	int isAnguloGradiente = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetAnguloGradiente));
	int isAnguloEscolha = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgetAnguloEscolha));
	
	if (angulo == 90){
		angulo = 0;
	}

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

			int pixel_x = ceil(ii+((nivel)/2));
			int pixel_y = ceil(jj+((nivel)/2));

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
		if(isAnguloAleatorio){
			pintarLadrilho(destino, ladrilhos[i], rand()%360);
		}else if(isAnguloGradiente){
			pintarLadrilhoGradiente(destino, origem, ladrilhos[i]);
		}else{
			pintarLadrilho(destino, ladrilhos[i], angulo);				
		}
	}

	free(ladrilhos);
	return destino;
}
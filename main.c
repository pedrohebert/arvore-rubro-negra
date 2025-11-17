#include <stdlib.h>
#include <stdio.h>
#include "arvRN.h"

#ifdef _WIN32
#define clear "cls"
#else
#define clear "clear"
#endif

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int menu(){
	puts("======================");
	puts("");
	puts("1 - inserir");
	puts("2 - remover");
	puts("3 - sair");
	puts("");
	
	int valor;
	
	printf("digite a opcao: ");
	scanf("%d", &valor);
	printf("%d", valor);
	return valor;
}

int main() {
	tree arv = NULL;
	int opc, valor;

	while(1){ //troca para 0 para testes
		opc = menu();
		system(clear);
		//printf("\033[2J\033[H"); //outra forma de limpar o terminal
		switch (opc) {
			case 1:{
				printf("digite o valor de insercao: ");
				scanf("%d", &valor);
				inserir(&arv, valor);
				desenharArvore(arv, 0);
				break;
			}
			case 2:{
				printf("digite o valor da remocao: ");
				scanf("%d", &valor);
				removerNo(&arv, valor);
				desenharArvore(arv, 0);
				break;
			}
			case 3:
				destruirArv(&arv);
				exit(0);
				break;
			default:
				printf("ERROR: opcao invalida, insira novamente");
		}
		puts("");
	}
	
	
	teste(arv);


	return 0;
}

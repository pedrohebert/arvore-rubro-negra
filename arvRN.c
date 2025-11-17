#include "arvRN.h"
#include <stdio.h>
#include <stdlib.h>



#define VER "\x1B[31m"
#define PRE "\x1B[90m"
#define RESET "\x1B[0m"

void corrigirInserir(tree *raiz, tree *curr);
void corrigirRemover(tree *raiz, tree *remover);
void desenharArvore(tree curr, int nivel);

int e_filho(tree pai, tree filho){
    if(pai->dir == filho) 
        return 1;
    else if (pai->esq == filho)
        return -1;
    else 
        return 0;
}

tree novo_no( int valor){
    tree novo = (tree)malloc(sizeof(no));
    novo->valor = valor;
    novo->cor = rubro;
    novo->dir = NULL;
    novo->esq = NULL;
    novo->pai = NULL;

    return novo;
}

void rotacaoEsq(tree *curr){
    tree atual = *curr;
    tree fDir = atual->dir;

    atual->dir = fDir->esq;

    if (fDir->esq)
        fDir->esq->pai = atual;

    fDir->esq = atual;

    fDir->pai = atual->pai;
    atual->pai = fDir;

    *curr = fDir; 
}

void rotacaoDir(tree *curr){
    tree atual = *curr;
    tree fEsq = atual->esq;

    atual->esq = fEsq->dir;

    if (fEsq->dir)
        fEsq->dir->pai = atual;

    fEsq->dir = atual;

    // atualiza pais
    fEsq->pai = atual->pai;
    atual->pai = fEsq;

    // atualiza o pontero 
    *curr = fEsq;  
}

void inserir(tree *raiz, int valor){
    tree atual = *raiz;
    tree pai = NULL;

    while (atual) {
        pai = atual;
        if (atual->valor < valor)
            atual = atual->dir;  
        else if (atual->valor > valor)
            atual = atual->esq; 
        else    
            return;
    }

    tree novo = novo_no(valor);
    novo->pai = pai;

    if (!pai)
        *raiz = novo;
    else if (valor < pai->valor)
        pai->esq = novo;      // menor → esquerda
    else
        pai->dir = novo;      // maior → direita

    corrigirInserir(raiz, &novo);
}


void corrigirInserir(tree *raiz, tree *curr){

    tree atual = *curr;

    if (!atual) return;

    //vinsinhasa
    tree pai = atual->pai;
    tree avo = NULL;
    tree tio = NULL;
    tree* pnt_avo = raiz;
    
    if(pai) avo = pai->pai;
    
    if (avo){
        if (avo->dir == pai)
            tio = avo->esq;
        else 
            tio = avo->dir;
        //ponteiro para avo
        if (avo->pai){
            if(avo->pai->esq == avo)
                pnt_avo = &avo->pai->esq;
            else
                pnt_avo = &avo->pai->dir; 
        }   
    }

    

    //regra 1: o no é a raiz
    if (!atual->pai){
        atual->cor = negro;
        return;
    }

    // subcaso 2 o no é filho do um no negro, não faz nada
    if (atual->pai->cor == negro) return;


    //caso 1 : 'atual' possui um tio rubro e 'pai' é filho rubro da esquerda
    
    if (avo){
//        puts("inicio");
//        desenharArvore(avo,0);
//        getchar();

        if ((tio && tio->cor == rubro)){
            tio->cor = negro;
            pai->cor = negro;
            avo->cor = rubro;

            
            if(!avo->pai || avo->pai->cor == rubro){
                corrigirInserir(raiz, &avo);
            }
        return;
//        puts("1e f");
//        desenharArvore(avo,0);
//        getchar();            
        }

        if (avo->esq == pai && (!tio || tio->cor == negro)){
            // caso 2de (direita-esquerda) : atual e um filho da direita, o pai é um filho rubro da esqueda e o tio é negro
            if(pai->dir == atual){
                rotacaoEsq(&avo->esq);
                //sempre vai cair no caso 3 depois; o pai e atual troca de lugar
                // atualiza o no atual
                corrigirInserir(raiz ,&atual->esq);
//                puts("2de f");
//                desenharArvore(avo,0);
//                getchar();
                return;
            }

            // caso 3e: atual e um filho da esquerda, o pai é um filho rubro da esqueda e o tio é negro

            rotacaoDir(pnt_avo);
            pai->cor = negro;
            avo->cor = rubro;
//              puts("3e f");
//              desenharArvore(avo,0);
//              getchar();
            return;
        }

        if (avo->dir == pai && (!tio || tio->cor == negro)){
        // caso 2ed (esquerda-direita): atual é filho da esquerda, pai é filho rubro da direita, tio é negro

            if (pai->esq == atual){
                rotacaoDir(&avo->dir);
                //atualiza o no atual
                corrigirInserir(raiz, &atual->dir);
    //           puts("2ed f");
    //            desenharArvore(avo,0);
    //            getchar();
                return;
            }

        //caso 3d: atual é filho da direita, pai é filho rubro da direita, tio é negro

                rotacaoEsq(pnt_avo);
                pai->cor = negro;
                avo->cor = rubro;
                //puts("3d f");
                //desenharArvore(avo,0);
                //getchar();
                return;
        }
    }
}





void removerNo(tree *raiz, int valor){
    tree atual = *raiz;
    tree aux = NULL;
    int troca;

    while(atual && atual->valor != valor){
        if (atual->valor < valor)
            atual = atual->dir;  
        else if (atual->valor > valor)
            atual = atual->esq; 
    }

    if (!atual) return;

    if (atual->dir && atual->esq) {
        aux = atual->esq;
        while(aux->dir) aux = aux->dir;
    }
    
    if(aux){
        troca = atual->valor;
        atual->valor = aux->valor;
        aux->valor = troca;
        atual = aux;
    }

    
    corrigirRemover(raiz, &atual);

    if(atual->pai){
        if (atual->pai->esq == atual) {
            atual->pai->esq = NULL;
        }
        if(atual->pai->dir == atual){
            atual->pai->dir = NULL;
        }
    }else{
        *raiz = NULL;
    }

    free(atual);
}


void corrigirRemover(tree *raiz, tree *remover){
    tree rem = *remover;
    tree pai = rem->pai;
    tree irmao = NULL;
    tree subrEsq = NULL;
    tree subrDir = NULL;

    // 1 = direita; 0 = não exite; -1 = esquerda
    int ladoRem = 0;

    tree *pnt_rem = NULL;
    tree *pnt_pai = NULL;
    tree *pnt_irm = NULL;

    if (pai) {
        if(pai->dir == rem){
            irmao = pai->esq;
            pnt_rem = &pai->dir;
            pnt_irm = &pai->esq;
            ladoRem = 1;
        }else{
            irmao = pai->dir;
            pnt_rem = &pai->esq;
            pnt_irm = &pai->dir;
            ladoRem = -1;
        }

        if(pai->pai){
            if(pai->pai->esq == pai)
                pnt_pai = &pai->pai->esq;
            else
                pnt_pai = &pai->pai->dir;
        }else {
            pnt_pai = raiz;
        }
    }else{
        pnt_rem = raiz;
    }

    if(irmao){
        subrDir = irmao->dir;
        subrEsq = irmao->esq;
    }

    
    
 
//    printf("inicio, alvo %d\n", rem->valor);
//    desenharArvore(pai,0);
//    getchar();


    //caso 0: rubro sem filho
    if(rem->cor == rubro){
        if(rem->pai->dir == rem)
            rem->pai->dir = NULL;
        else
            rem->pai->esq = NULL;
        return;
    }

    //caso 0 negro: rem é negro e tem apena um filho vermelho
    
    //caso 0d
    if(rem->dir && rem->dir->cor == rubro && !rem->esq){
        rem->dir->cor = negro;
        rotacaoEsq(pnt_rem);
        
        return;
    }

    //caso 0e
    if(rem->esq && rem->esq->cor == rubro && !rem->dir){
        rem->esq->cor = negro;
        rotacaoDir(pnt_rem);

        return;
    }
    
//    printf("caso 0, alvo %d\n", rem->valor);
//    desenharArvore(pai,0);
//    getchar();

        
    
    if (pai && irmao){
        //caso 1: o irmão de rem é rubro
        if( irmao->cor == rubro){
            irmao->cor =negro;
            pai->cor = rubro;
            if (ladoRem == 1)
                rotacaoDir(pnt_pai);
            else 
                rotacaoEsq(pnt_pai);

            corrigirRemover(raiz, pnt_rem);
            return;
        }
//        printf("caso 1d, alvo %d\n", rem->valor);
//        desenharArvore(pai,0);
//        getchar();
    

        
        //caso 2: irmão é negro e filhos dele são negros
        if (ladoRem == 1){// direita
            //caso 2d: irmão é negro e filhos dele são negros
            if((!subrDir || subrDir->cor == negro) && (!subrEsq || subrEsq->cor == negro)){
                irmao->cor = rubro;
                if (pai->cor == negro){
                    corrigirRemover(raiz, pnt_pai);
                    return;
                }
                pai->cor = negro;
                return;
            }
            
//            printf("caso 2d, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();

            //caso 3d: sobrinho esquerda (proximo) é rubro e subrino esqueda (distante) é negro
            if (subrDir && subrDir->cor == rubro && (!subrEsq || subrEsq->cor == negro)){
                subrDir->cor = negro;
                irmao->cor = rubro;
                rotacaoDir(pnt_irm);
                corrigirRemover(raiz, pnt_rem);
                return;
            }
            
//            printf("caso 3d, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();

            // caso 4d: subrino da esquerda (distante) é rubro
            if (subrEsq && subrEsq->cor == rubro){
                irmao->cor = pai->cor;
                pai->cor = negro;
                subrEsq->cor = negro;
                rotacaoDir(pnt_pai);
                return;
            }
//            printf("caso 4d, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();
        }

        if (ladoRem == -1) {//esquerda
            //caso 2e: irmão é negro e filhos dele são negros
            if(irmao && (!subrEsq || subrEsq->cor == negro) && (!subrDir || subrDir->cor == negro)){
                irmao->cor = rubro;
                if (pai->cor == negro){
                    corrigirRemover(raiz, pnt_pai);
                    return;
                }
                pai->cor = negro;
                return;
            } 
//            printf("caso 2e, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();

            //caso 3e: sobrinho esquerda (proximo) é rubro e subrino direita (distante) e negro  

            if (subrEsq && subrEsq->cor == rubro && (!subrDir || subrDir->cor == negro)){
                subrDir->cor = negro;
                irmao->cor = rubro;
                rotacaoEsq(pnt_irm);
                corrigirRemover(raiz, pnt_rem);
                return;
            }
//            printf("caso 3e, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();

            // caso 4e: subrino da direita (distante) é rubro
            if (subrDir && subrDir->cor == rubro){
                irmao->cor = pai->cor;
                pai->cor = negro;
                subrDir->cor = negro;
                rotacaoEsq(pnt_pai);
                return;
            }
//            printf("caso 4e, alvo %d\n", rem->valor);
//            desenharArvore(pai,0);
//            getchar();
        }
       
    }

}




void destruirArv(tree* arv){
    tree curr = *arv;

    if (curr->dir) destruirArv(&curr->dir);
    if (curr->esq) destruirArv(&curr->esq);
    free(curr);
}


void desenhar(tree raiz){
    if(!raiz) return;
    
    desenhar(raiz->dir);
    printf(" %d ", raiz->valor );
    desenhar(raiz->esq);
}



void desenharArvore(tree curr, int nivel) {
    // Se o nó for nulo, retorne
    if (curr == NULL) {
        return;
    }

    // Imprime o filho direito com indentação
    desenharArvore(curr->dir, nivel + 1);
    tree pai = curr->pai;
    tree avo = NULL;
    if (pai) avo = pai->pai;

    // Imprime o nó atual com a indentação correta
    for (int i = 0; i < nivel; i++) {


        if(i == nivel-1 && pai){
            printf("+ - - ");
        }else if (i == nivel - 2 && pai && avo){
            if (curr->valor < pai->valor &&  avo->valor < curr->valor){
                printf("|     ");
            }else if (curr->valor > pai->valor &&  avo->valor > curr->valor){
                printf("|     ");
            }else {
                printf("      "); // Indentação
            }
        }else{
            printf("      "); // Indentação
        }
    }
    
    printf("%s%d\n"RESET,
        ((curr->cor == rubro)? VER: PRE), 
        curr->valor);

    // Imprime o filho esquerdo com indentação
    desenharArvore(curr->esq, nivel + 1);
}

void teste(tree arv){

    inserir(&arv, 0);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 1);
    puts("raiz");
    desenharArvore(arv, 0);
    getchar();
    inserir(&arv, 2);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 3);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 4);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 5);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 6);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 7);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 8);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 9);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 10);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 11);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 12);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 13);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 15);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 16);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 17);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 18);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 19);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    inserir(&arv, 20);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();

    removerNo(&arv, 4);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    removerNo(&arv, 3);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    removerNo(&arv, 2);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    removerNo(&arv, 1);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
    removerNo(&arv, 5);
    puts("raiz");
    desenharArvore(arv,0);
    getchar();
 

    
}
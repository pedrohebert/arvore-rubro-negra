
#define rubro 1
#define negro 0

typedef struct no* tree;

typedef struct no{
    int valor;
    tree esq;
    tree dir;
    tree pai;
    
    int cor;
} no;



void inserir(tree *raiz, int valor);

void removerNo(tree *raiz, int valor);

void desenhar(tree raiz);

void desenharArvore(tree curr, int nivel);

void teste(tree arv);

void destruirArv(tree* arv);
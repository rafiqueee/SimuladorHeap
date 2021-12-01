#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>

#define HEAP_SIZE 100

typedef struct  elemento
{
    char id;
    int tamanho;
    int ini;
}elemento;

typedef struct listaElementos
{
    elemento *obj;
    struct listaElementos *prox;
}listaElementos;

void Inicializar(bool *heap);
void Imprimir(bool *heap, listaElementos *lista, bool *heapLixo);
void ModoHeap(char *metodo);
void Adicionar(char id, int tamanho, bool *heap, listaElementos **lista);
void Deletar(char id, bool *heap, listaElementos **lista);
void Atribuir(elemento *a, elemento *b, bool *heapLixo);
void Interpretar(bool *heap, bool *heapLixo, listaElementos **lista);

HANDLE hConsole;
int ind = 0;
int cont = 0;
int modo = 0;


int main()
{
    setlocale(LC_CTYPE, "Portuguese");
    
    bool heap[HEAP_SIZE];
    bool heapLixo[HEAP_SIZE];
    listaElementos *lista = NULL;
    listaElementos *aux;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Inicializar(heap);
    Inicializar(heapLixo);
    
    while(cont == 0)
    {
        Interpretar(heap, heapLixo, &lista);
    }
}

elemento *CriaElemento(char id, int tamanho)
{
    elemento *new = (elemento*) malloc(sizeof(elemento));
    new->id = id;
    new->tamanho = tamanho;
    new->ini = -1;

    return new;
}
void Inicializar(bool *heap)
{
    for(int i = 0; i < HEAP_SIZE; i++)
        heap[i] = false;   
}
void Imprimir(bool *heap, listaElementos *lista, bool *heapLixo)
{
    printf("\n");
    CONSOLE_SCREEN_BUFFER_INFO def;
    GetConsoleScreenBufferInfo(hConsole, &def);
    for(int i = 0; i < HEAP_SIZE; i++)
    {
        if(i != 0 && i % 20 == 0)
            printf("\n");
        if(heap[i])
            SetConsoleTextAttribute(hConsole, 32);
        else
            SetConsoleTextAttribute(hConsole, 135);
        if(heapLixo[i])
            SetConsoleTextAttribute(hConsole, 79);
        printf("%3d  ", i);
    }
    SetConsoleTextAttribute(hConsole, def.wAttributes);
    printf("\n");
    while(lista != NULL)
    {
        elemento *obj = lista->obj;
        printf("%c %d - %d\n", obj->id, obj->ini, obj->ini + obj->tamanho - 1);
        lista = lista->prox;
    }
}
void ModoHeap(char *metodo)
{
    if(strcmp(metodo, "next") == 0)
    {
        modo = 1;
    }
    else if(strcmp(metodo, "first") == 0)
    {
        modo = 0;
        ind = 0;
    }
}
void AdicionarNaLista(elemento *obj, listaElementos **lista)
{
    listaElementos *new = (listaElementos*) malloc(sizeof(listaElementos));
    new->obj = obj;
    new->prox = NULL;
    if(*lista == NULL)
        *lista = new;
    else
    {
        listaElementos *aux = *lista;
        while(aux->prox != NULL)
            aux = aux->prox;
        aux->prox = new;
    }
}
listaElementos *RemoverDaLista(elemento *obj, listaElementos *lista)
{
    if(lista->obj == obj)
    {
        listaElementos *aux = lista->prox;
        free(lista->obj);
        free(lista);
        return aux;
    }else
        lista->prox = RemoverDaLista(obj, lista->prox);
        return lista;
}
void Adicionar(char id, int tamanho, bool *heap, listaElementos **lista)
{
    elemento *obj = CriaElemento(id, tamanho);
    int i;
    if(modo == 1)
        i = ind;
    else
        i = 0;

    bool test = false;
    int ini = -1;
    while(i < HEAP_SIZE && ini == -1)
    {
        if(heap[i] == false && i + obj->tamanho <= HEAP_SIZE)
        {
            for(int j = 0; j < obj->tamanho; j++)
                test = test || heap[j + i];
            if(!test)
                ini = i;
        }
        i++;
    }
    if(ini != -1)
    {
        for(int j = 0; j < obj->tamanho; j++)
            heap[ini + j] = true;
        obj->ini = ini;
        ind = ini + obj->tamanho;
        AdicionarNaLista(obj, lista);
    }
}
void Deletar(char id, bool *heap, listaElementos **lista)
{
    listaElementos *aux = *lista;
    while(aux->obj->id != id)
        aux = aux->prox;
    for(int i = aux->obj->ini; i < aux->obj->ini + aux->obj->tamanho; i++)
        heap[i] = false;
    int ini = aux->obj->ini;
    *lista = RemoverDaLista(aux->obj, *lista);
    aux = *lista;
    while(aux != NULL)
    {
        if(aux->obj->ini == ini)
        {
            *lista = RemoverDaLista(aux->obj, *lista);
            aux = *lista;
        }else
            aux = aux->prox;
    }
}
void Atribuir(elemento *a, elemento *b, bool *heapLixo)
{
    for(int i = 0; i < a->tamanho; i++)
    {
        heapLixo[i + a->ini] = true;
    }
    a->ini = b->ini;
    a->tamanho = b->tamanho;
}
void Interpretar(bool *heap, bool *heapLixo, listaElementos **lista)
{
    char cmd[15];
    char aux[15];

    printf("\n> ");
    fgets(cmd, 15, stdin);
    cmd[strlen(cmd) - 1] = '\0';

    int j;
    for(j = 0; cmd[j] != ' ' && cmd[j] != '\0'; j++)
        aux[j] = cmd[j];
    aux[j] = '\0';

    if(strcmp(aux, "new") == 0)
    {
        j++;
        char id = cmd[j];
        j++; j++;
        int i;
        for(i = 0; cmd[j + i] != '\0'; i++)
            aux[i] = cmd[j + i];
        aux[i] = '\0';
        int tamanho = atoi(aux);
        Adicionar(id, tamanho, heap, lista);
    }else if (strcmp(aux, "heap") == 0)
    {
        j++;
        int i;
        for(i = 0; cmd[j + i] != '\0'; i++)
            aux[i] = cmd[j + i];
        aux[i] = '\0';
        ModoHeap(aux);
    }else if (strcmp(aux, "exibe") == 0)
        Imprimir(heap, *lista, heapLixo);
    else if (strcmp(aux, "del") == 0)
    {
        j++;
        char id = cmd[j];
        Deletar(id, heap, lista);
    }else if(cmd[j + 1] == '=')
    {
        char *par2 = cmd + j + 3;
        listaElementos *auxlista = *lista;
        elemento *a, *b;
        while(auxlista != NULL)
        {
            if(auxlista->obj->id == aux[0])
                a = auxlista->obj;
            if(auxlista->obj->id == par2[0])
                b = auxlista->obj;
            auxlista = auxlista->prox;
        }
        Atribuir(a, b, heapLixo);
    }else if(strcmp(aux, "exit") == 0)
    {
        cont = 1;
    }else
        printf("Comando %c%s%c n�o reconhecido\n", 34, aux, 34);
}
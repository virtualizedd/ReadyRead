#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//Nodo de una lista enlazada de palabras
struct palabra
{
    char pal[30];
    int frq;
    palabra* sig = NULL;
}

//Función para definir una nueva lista de palabras
//retorna un puntero al primer elemento

palabra* PalabraList(char valor[30])
{
    palabra* inicio = new palabra();
    inicio -> pal = valor;
    return  inicio;
}

// -- MAIN --

int main()
{
    ifstream archivo('ruta.txt', ios::binary);
    
    
    return 0;
}

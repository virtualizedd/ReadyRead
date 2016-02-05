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

//Función que crea un nuevo nodo para una lista de palabras
//@return: puntero al nodo creado

palabra* nuevoNodo(char valor[30])
{
    palabra* nodo = new palabra;
    strcpy(nodo -> pal, valor);
    nodo -> freq = 1;
    
    return nodo;
}


//Función para inicializar una nueva lista de palabras
//@return: puntero al que será el primer elemento

palabra* PalabraList(char valor[30])
{
    palabra* inicio = nuevoNodo(valor);
    return  inicio;
}

// -- MAIN --

int main()
{
    ifstream archivo('ruta.txt', ios::binary);
    
    char c[30];
    
    //Inicialización de la lista que almacenará todas las palabras
    //presentes en el documento.
    archivo >> c;
    palabra* lista_palabras = PalabraList(c)
    
    archivo.close();
    
    return 0;
}

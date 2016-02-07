#include <iostream>
#include <fstream>
using namespace std;

//Estructura para una palabra
struct palabra
{
    char palabra[30];
    int frq;
    palabra* sig;
}

//Nodo para un árbol
struct arbol
{
    char Letra;
    palabra* l;
    arbol* ri;
    arbol* rd;
}

// -- MAIN --

int main()
{
    return 0;
}

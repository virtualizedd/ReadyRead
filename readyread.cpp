#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//Nodo de una lista enlazada de palabras
struct palabra
{
    char pal[30];
    int freq;
    palabra* sig = NULL;
};

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

//Función que construye una lista enlazada de nodos palabras en orden alfabético
//a partir del contenido de un archivo de texto.
//@return: puntero al inicio de la lista

/*palabra* ConstruirLista(char ruta[255])
{
    ifstream archivo(ruta, ios::binary);
    
    char c[30];
    archivo >> c;
    
    palabra* nueva_lista = PalabraList(c);
    
    archivo.close();
}*/

//Función que genera un menú de inicio y pide al usuario la ruta del archivo
//@return: <void>

void menuInicio(char ruta[255])
{
    cout << "|||||| ReadyRead v.0.0.1 - inspector de archivos |||||||||||||||||\n"
         << "------------------------------------------------------------------\n"
         << "                                                                  \n"
         << "Ingrese la ruta del archivo que desea analizar:                  \n"
         << ">>> ";
    
    cin >> ruta;
}
         
// -- MAIN --

int main()
{
    char ruta_archivo[255];
    
    menuInicio(ruta_archivo);
    
    cout << ruta_archivo;
    
    return 0;
}

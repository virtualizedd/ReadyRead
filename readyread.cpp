#include <iostream>
#include <fstream>
#include <iomanip>
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

//Función que inserta un elemento respetando un orden alfabetico
//Si el valor ya se encuentra en la lista, aumenta en uno su atributo freq
//@return: un puntero al inicio de la lista
palabra* insert_alfabetico(palabra* lista, char valor[30]){
     
    int comparacion = strcmp(lista -> pal, valor);
    
    if(comparacion == 0){
        lista -> freq += 1;
        return lista;
    }
    else if(comparacion > 0){
        palabra* nuevo = nuevoNodo(valor);
        nuevo -> sig = lista;
        return nuevo;
    }
    else{
        palabra* inicio = lista;
        palabra* anterior = lista;
        lista = lista -> sig;
        
        while(lista != NULL){
            comparacion = strcmp(lista -> pal, valor);
            
            if(comparacion == 0){
                   lista -> freq += 1; 
                   return inicio;
            }
            else if(comparacion > 0){
                palabra* nuevo = nuevoNodo(valor);
                
                nuevo -> sig = lista;
                anterior -> sig = nuevo;
                return inicio;
            }
            
            anterior = lista;
            lista = lista -> sig;
        }
        
        palabra* nuevo = nuevoNodo(valor);
        anterior -> sig = nuevo;
        return inicio;
    }
}


//Función que construye una lista enlazada de nodos palabras en orden alfabético
//a partir del contenido de un archivo de texto.
//@return: puntero al inicio de la lista

palabra* ConstruirLista(char ruta[255])
{
    ifstream archivo(ruta, ios::binary);
    
    char c[30];
    archivo >> c;
    
    palabra* nueva_lista = PalabraList(c);
    
    while(archivo >> c){
        nueva_lista = insert_alfabetico(nueva_lista, c);
    }
    
    archivo.close();
    
    return nueva_lista;
}

//Función que genera un menú de inicio y pide al usuario la ruta del archivo
//@return: <void>

void menuInicio(char ruta[255])
{
    cout << "|||||| ReadyRead v.0.0.1 - inspector de archivos |||||||||||||||||\n"
         << "------------------------------------------------------------------\n"
         << "                                                                  \n"
         << "Ingrese la ruta del archivo que desea analizar:                   \n"
         << ">>> ";
    
    cin >> ruta;
    
    cout << "\n\n";
}

//Función que imprime una tabla con todas las palabras de una lista junto
//a su frecuencia.
//@return: <void>

void imprimirTablaPalabras(palabra* lista)
{
    cout << "          palabra          ||  frecuencia   \n"
         << "============================================\n";
         
    while(lista  != NULL){
        cout << setw(30) << setfill(' ') << left << lista -> pal
             << "     "  << lista -> freq << "\n"
             << "--------------------------------------------\n";
        lista = lista -> sig;
    }
    
}
         
// -- MAIN --

int main()
{
    char ruta_archivo[255];
    
    menuInicio(ruta_archivo);
    
    palabra* lista_palabras = ConstruirLista(ruta_archivo);
    
    imprimirTablaPalabras(lista_palabras);
    
    return 0;
}

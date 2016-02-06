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

//Función que verifica si un carácter corresponde a un signo de puntuación
//@return: booleano

bool esSigno(char c)
{
    char signos[30] = "\"',;.()[]{}#:-_¡!¿?";
    short int i = 0;
    
    while(signos[i])
    {
        if(signos[i] == c) return true;
        i++;
    }
    
    return false;
}

//Función que convierte a minúsculas todos los carácteres de un array
//@return: <void>

void formatearPalabra(char p[30])
{
    short int i, j = 0;
    char formato[30] = "";
    
    while(p[i]){
        if(!esSigno(p[i])){
            formato[j] = tolower(p[i]);
            j++;
        }
        i++;
    }
    
    strcpy(p, formato);
}


//Función que construye una lista enlazada de nodos palabras en orden alfabético
//a partir del contenido de un archivo de texto.
//@return: puntero al inicio de la lista

palabra* ConstruirLista(char ruta[255])
{
    ifstream archivo(ruta, ios::binary);
    
    char c[30];
    archivo >> c;
    
    formatearPalabra(c);
    
    palabra* nueva_lista = PalabraList(c);
    
    while(archivo >> c){
        formatearPalabra(c);
        nueva_lista = insert_alfabetico(nueva_lista, c);
    }
    
    cout << "El archivo se ha leído correctamente.";
    
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
    
    cout << "\n\n";
    
}

//Función que imprime un menú con las distintas funcionalidades que ofrece
//el programa

void menuOpciones(palabra* lista)
{
    while(true){
        cout << "¿Qué deseas hacer?\n\n"
             << "   [1] Listar todas las palabras presentes en el archivo\n"
             << "       junto a su frecuencia de aparición\n"
             << "   [2] Saber cuáles son las palabras más y menos frecuentes                  \n"
             << "   [3] Saber cuáles son las palabras con mayor y menor número de carácteres  \n"
             << "   [n] Ingresar una nueva ruta \n"
             << "   [x] Salir de la aplicación \n\n"
             << "Ingresa el [<codigo>] listado a la izquierda de la opción que deseas ejecutar\n"
             << ">>> ";
        char opcion;
        
        cin >> opcion;
        
        cout << "\n\n";
        
        if(opcion == 'x')  break;
        else if(opcion == 'n'){
        }
        else if(opcion == '1') imprimirTablaPalabras(lista);
        
        cout << "¿Deseas realizar una nueva operación sobre este archivo?\n"
             << "[s]i    [m]e gustaria analizar otro archivo    [n]o, quiero salir del programa \n"
             << ">>> ";
             
       cin >> opcion;
       
       if(opcion == 's') cout << "\n\n";
       else if(opcion == 'm'){
       }
       else break;
       
       
    }
}
         
// -- MAIN --

int main()
{
    char ruta_archivo[255];
    
    menuInicio(ruta_archivo);
    
    palabra* lista_palabras = ConstruirLista(ruta_archivo);
    
    menuOpciones(lista_palabras);
    
    return 0;
}

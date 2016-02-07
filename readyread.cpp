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

//Nodo para un árbol binario
struct arbol
{
    char Letra;
    palabra* lista;
    arbol* ri;
    arbol* rd;
};

//Función que crea un nuevo nodo para una lista de palabras
//@return: puntero al nodo creado

palabra* nuevoNodoPalabra(char valor[30])
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
    palabra* inicio = nuevoNodoPalabra(valor);
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
        palabra* nuevo = nuevoNodoPalabra(valor);
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
                palabra* nuevo = nuevoNodoPalabra(valor);
                
                nuevo -> sig = lista;
                anterior -> sig = nuevo;
                return inicio;
            }
            
            anterior = lista;
            lista = lista -> sig;
        }
        
        palabra* nuevo = nuevoNodoPalabra(valor);
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

//Función que define un nuevo nodo para un ABO
//@return: puntero al nodo definido

arbol* nuevoNodoArbol(char p[30])
{   
    palabra* nueva_lista = PalabraList(p);
    arbol* nodo = new arbol;
    nodo -> Letra = p[0];
    nodo -> lista = nueva_lista;
    
    return nodo;
}

//Función que inicializa un ABO
//@return: puntero al nodo inicial del árbol

arbol* nuevoArbol(char p[30])
{
    arbol* inicio =  nuevoNodoArbol(p);
    return inicio;
}

//Función que inserta un nuevo valor al ABO según corresponda
//@return: nodo de inicio

arbol* insert_ABO(arbol* abo,char p[30])
{
    arbol* inicio = abo;
    
    while(true){
        
        if(p[0] == abo -> Letra){
            abo -> lista = insert_alfabetico(abo -> lista, p);
            break;
        }
        else if(p[0] < abo -> Letra){
            if(abo -> ri == NULL){
                arbol* nodo = nuevoNodoArbol(p);
                abo -> ri = nodo;
                break;
            }
            else abo = abo -> ri;
        }
        else{
            if(abo -> rd == NULL){
                arbol* nodo = nuevoNodoArbol(p);
                abo -> rd = nodo;
                break;
            }
            else abo = abo -> rd;
        }
    }
    
    return inicio;
}

//Función que construye una lista enlazada de nodos palabras en orden alfabético
//a partir del contenido de un archivo de texto.
//@return: puntero al inicio de la lista

arbol* ConstruirABO(char ruta[255])
{
    ifstream archivo(ruta, ios::binary);
    
    char c[30];
    archivo >> c;
    formatearPalabra(c);

    arbol* nuevo_arbol = nuevoArbol(c);
    
    while(archivo >> c){
        formatearPalabra(c);
        nuevo_arbol = insert_ABO(nuevo_arbol, c);
    }
    
    cout << "El archivo se ha leído correctamente.";
    
    archivo.close();
    
    return nuevo_arbol;
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

void imprimirListaPalabras(palabra* lista)
{      
    while(lista  != NULL){
        cout << " " << setw(30) << setfill(' ') << left <<  lista -> pal
             << "     "  <<  lista -> freq << "\n"
             << "--------------------------------------------\n";
        lista = lista -> sig;
    }
}

//Función que recorre un arbol y imprime las listas de palabras de cada nodo
//en orden alfabético.
//@return: <void>

void imprimirListasArbol(arbol* abo)
{
    if(abo -> ri == NULL)
    {
        imprimirListaPalabras(abo -> lista);
    }
    else
    {
        imprimirListasArbol(abo -> ri);
        imprimirListaPalabras(abo -> lista);
    }
    
    if(abo -> rd != NULL)
    {
        imprimirListasArbol(abo -> rd);
    }
}

//Función que imprime un menú con las distintas funcionalidades que ofrece
//el programa

void menuOpciones(arbol* abo)
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
        else if(opcion == '1'){
            cout << "           palabra          ||  frecuencia   \n"
                 << "============================================\n";
            imprimirListasArbol(abo);
            
            cout << "\n\n";
        }
        
        cout << "¿Deseas realizar una nueva operación sobre este archivo?\n"
             << "[s]i    [m]e gustaria analizar otro archivo    [n]o, quiero salir del programa \n"
             << ">>> ";
             
       cin >> opcion;
       
       if(opcion == 's') cout << "\n\n";
       else if(opcion == 'm'){
           cout << "\n";
       }
       else break;
       
       
    }
}
         
// -- MAIN --

int main()
{
    char ruta_archivo[255];
    
    menuInicio(ruta_archivo);
    
    arbol* arbol_palabras = ConstruirABO(ruta_archivo);
    
    menuOpciones(arbol_palabras);

    
    return 0;
}

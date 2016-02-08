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
    palabra* l;
    arbol* ri = NULL;
    arbol* rd = NULL;
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

//Función que convierte a minúsculas todos los carácteres de un array y quita
//los signos de puntuación.
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
    palabra* nueva_lista = nuevoNodoPalabra(p);
    arbol* nodo = new arbol;
    nodo -> Letra = p[0];
    nodo -> l = nueva_lista;
    
    return nodo;
}

//Función que inserta un nuevo valor al ABO según corresponda
//@return: nodo de inicio

arbol* insert_ABO(arbol* abo,char p[30])
{
    arbol* inicio = abo;
    
    while(true){
        
        if(p[0] == abo -> Letra){
            abo -> l = insert_alfabetico(abo -> l, p);
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

//Función que elimina una lista enlazada de palabras

void eliminarLista(palabra* ls)
{
    palabra* anterior = new palabra;
    
    while(ls != NULL){
        anterior = ls;
        ls = ls -> sig;
        delete anterior;
    }
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

    arbol* nuevo_arbol = nuevoNodoArbol(c);
    
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
        imprimirListaPalabras(abo -> l);
    }
    else
    {
        imprimirListasArbol(abo -> ri);
        imprimirListaPalabras(abo -> l);
    }
    
    if(abo -> rd != NULL)
    {
        imprimirListasArbol(abo -> rd);
    }
}


//Función que retorna una lista enlazada con las palabras menos frecuentes
//@return: puntero al primer elemento de la lista

palabra* menosFrecuentes(arbol* abo, palabra* minFreq)
{
    if(abo -> ri == NULL){
        palabra* ls_pal =  abo -> l;
        while(ls_pal != NULL){
            if((ls_pal -> freq) < (minFreq -> freq)){
                eliminarLista(minFreq -> sig);
                minFreq -> freq = ls_pal -> freq;
                minFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minFreq -> sig -> freq = minFreq -> freq;
            }
            else if((ls_pal -> freq) == (minFreq -> freq)){
                palabra* inicio = minFreq;
                while(minFreq -> sig != NULL) minFreq = minFreq -> sig;
                minFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minFreq -> sig -> freq = minFreq -> freq;
                minFreq = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    else{
        
        minFreq = menosFrecuentes(abo -> ri, minFreq);
        palabra* ls_pal =  abo -> l;
        
        while(ls_pal != NULL){
            
            if((ls_pal -> freq) < (minFreq -> freq)){
                eliminarLista(minFreq -> sig);
                minFreq -> freq = ls_pal -> freq;
                minFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minFreq -> sig -> freq = minFreq -> freq;
            }
            else if((ls_pal -> freq) == (minFreq -> freq)){
                palabra* inicio = minFreq;
                while(minFreq -> sig != NULL) minFreq = minFreq -> sig;
                minFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minFreq -> sig -> freq = minFreq -> freq;
                minFreq = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    
    if(abo -> rd != NULL) minFreq = menosFrecuentes(abo -> rd, minFreq);
    
    return minFreq;
}

//Función que retorna una lista enlazada con las palabras más frecuentes
//@return: puntero al primer elemento de la lista

palabra* masFrecuentes(arbol* abo, palabra* maxFreq)
{
    if(abo -> ri == NULL){
        palabra* ls_pal =  abo -> l;
        while(ls_pal != NULL){
            if((ls_pal -> freq) > (maxFreq -> freq)){
                eliminarLista(maxFreq -> sig);
                maxFreq -> freq = ls_pal -> freq;
                maxFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                maxFreq -> sig -> freq = maxFreq -> freq;
            }
            else if((ls_pal -> freq) == (maxFreq -> freq)){
                palabra* inicio = maxFreq;
                while(maxFreq -> sig != NULL) maxFreq = maxFreq -> sig;
                maxFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                maxFreq -> sig -> freq = maxFreq -> freq;
                maxFreq = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    else{
        
        maxFreq = masFrecuentes(abo -> ri, maxFreq);
        palabra* ls_pal =  abo -> l;
        
        while(ls_pal != NULL){
            
            if((ls_pal -> freq) > (maxFreq -> freq)){
                eliminarLista(maxFreq -> sig);
                maxFreq -> freq = ls_pal -> freq;
                maxFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                maxFreq -> sig -> freq = maxFreq -> freq;
            }
            else if((ls_pal -> freq) == (maxFreq -> freq)){
                palabra* inicio = maxFreq;
                while(maxFreq -> sig != NULL) maxFreq = maxFreq -> sig;
                maxFreq -> sig = nuevoNodoPalabra(ls_pal -> pal);
                maxFreq -> sig -> freq = maxFreq -> freq;
                maxFreq = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    
    if(abo -> rd != NULL) maxFreq = masFrecuentes(abo -> rd, maxFreq);
    
    return maxFreq;
}

//Función que retorna una lista enlazada con las palabras de menor longitud
//@return: puntero al primer elemento de la lista

palabra* menosCaracteres(arbol* abo, palabra* minLong)
{
    
    if(abo -> ri == NULL){
        palabra* ls_pal =  abo -> l;
        while(ls_pal != NULL){
            if(strlen(ls_pal -> pal) < unsigned(minLong -> freq)){
                eliminarLista(minLong -> sig);
                minLong -> freq = strlen(ls_pal -> pal);
                minLong -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minLong -> sig -> freq = ls_pal -> freq;
            }
            else if(strlen(ls_pal -> pal) == unsigned(minLong -> freq)){
                palabra* inicio = minLong;
                while(minLong -> sig != NULL) minLong = minLong -> sig;
                minLong -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minLong -> sig -> freq = ls_pal -> freq;
                minLong = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    else{
        
        minLong = menosCaracteres(abo -> ri, minLong);
        palabra* ls_pal =  abo -> l;
        
        while(ls_pal != NULL){
            if(strlen(ls_pal -> pal) < unsigned(minLong -> freq)){
                eliminarLista(minLong -> sig);
                minLong -> freq = strlen(ls_pal -> pal);
                minLong -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minLong -> sig -> freq = ls_pal -> freq;
            }
            else if(strlen(ls_pal -> pal) == unsigned(minLong -> freq)){
                palabra* inicio = minLong;
                while(minLong -> sig != NULL) minLong = minLong -> sig;
                minLong -> sig = nuevoNodoPalabra(ls_pal -> pal);
                minLong -> sig -> freq = ls_pal -> freq;
                minLong = inicio;
            }
            ls_pal = ls_pal -> sig;
        }
    }
    
    if(abo -> rd != NULL) minLong = menosCaracteres(abo -> rd, minLong);
    
    return minLong;
}

//Función que imprime un menú con las distintas funcionalidades que ofrece
//el programa.

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
        
        cout << "\n";
        
        if(opcion == 'x')  break;
        else if(opcion == 'n'){
        }
        else if(opcion == '1'){
            cout << "           palabra          ||  frecuencia   \n"
                 << "============================================\n";
            imprimirListasArbol(abo);
            
            cout << "\n\n";
        }
        else if(opcion == '2'){
            char inicio[30] = "_init_";
            palabra* max = nuevoNodoPalabra(inicio);
            palabra* min = nuevoNodoPalabra(inicio);
            max = masFrecuentes(abo, max);
            min = menosFrecuentes(abo, min);
            cout << " palabras más repetidas     ||  frecuencia   \n"
                 << "============================================\n";
                 
            imprimirListaPalabras(max -> sig);
            
            cout << "\n\n"
                 << " palabras menos repetidas   ||  frecuencia   \n"
                 << "============================================\n";
                 
            imprimirListaPalabras(min -> sig);
            
            cout << "\n\n";
            
            eliminarLista(max);
            eliminarLista(min);
        }
        else if(opcion == '3'){
            char inicio[30] = "_init_";
            palabra* max = nuevoNodoPalabra(inicio);
            palabra* min = nuevoNodoPalabra(inicio);
            min -> freq = 30;
            //max = masCaracteres(abo, max);
            min = menosCaracteres(abo, min);
            
            /*cout << " palabras más repetidas     ||  frecuencia   \n"
                 << "============================================\n";
                 
            imprimirListaPalabras(max -> sig);
            */
            
            cout << "\n\n"
                 << "Las palabras más cortas del documento tienen " << min -> freq << " caracter(es).\n\n"
                 << " palabras más cortas        ||   frecuencia \n"
                 << "============================================\n";
                 
            imprimirListaPalabras(min -> sig);
            
            cout << "\n\n";
            
            eliminarLista(max);
            eliminarLista(min);
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

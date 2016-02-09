#include <iostream> //I/O
#include <fstream>  //Lectura y Escritura de archivos
#include <iomanip>  //Manipuladores de formato para la salida
#include <string.h> //Funcionalidades para arrays de carácteres

using namespace std;

/************** STRUCTS ******************************************************/

//Nodo de una lista enlazada de palabras
struct palabra
{
    char pal[30];
    int freq;
    palabra* sig = NULL;
};

//Nodo para un árbol binario de listas enlazadas
struct arbol
{
    char Letra;
    palabra* l;
    arbol* ri = NULL;
    arbol* rd = NULL;
};

/*****************************************************************************/

/**************** FUNCIONES **************************************************/

/* @func: define en memoria un nuevo nodo de tipo palabra 
 * @params: p => Array de carácteres a asignar al atributo pal del nodo 
 * @return: puntero al nodo creado */
 
palabra* nuevoNodoPalabra(char p[30])
{
    palabra* nodo = new palabra;
    strcpy(nodo -> pal, p);
    nodo -> freq = 1;
    
    return nodo;
}

/* @func: crea un nuevo nodo de tipo palabra y lo inserta en una lista enlazada
 *        de palabras respetando un criterio de orden alfabético
 * @params: lista => lista ordenada alfabéticamente en la cual insertar el nodo
 *          p => Array de carácteres a asignar al atrib. pal del nodo creado
 * @return: puntero al primer elemento de la lista enlazada */
 
palabra* insert_alfabetico(palabra* lista, char p[30]){
     
    int comparacion = strcmp(lista -> pal, p);
    
    if(comparacion == 0){
        lista -> freq += 1;
        return lista;
    }
    else if(comparacion > 0){
        palabra* nuevo = nuevoNodoPalabra(p);
        nuevo -> sig = lista;
        return nuevo;
    }
    else{
        palabra* inicio = lista;
        palabra* anterior = lista;
        lista = lista -> sig;
        
        while(lista != NULL){
            comparacion = strcmp(lista -> pal, p);
            
            if(comparacion == 0){
                   lista -> freq += 1; 
                   return inicio;
            }
            else if(comparacion > 0){
                palabra* nuevo = nuevoNodoPalabra(p);
                
                nuevo -> sig = lista;
                anterior -> sig = nuevo;
                return inicio;
            }
            
            anterior = lista;
            lista = lista -> sig;
        }
        
        palabra* nuevo = nuevoNodoPalabra(p);
        anterior -> sig = nuevo;
        return inicio;
    }
}

/* @func: Verificar si un carácter es o no un signo de puntuación
 * @params: c => Carácter a evaluar
 * @return: un booleano que indica si el carácter es un signo o no */

bool esSigno(char c)
{
    char signos[30] = "\"',;.()[]{}#:-_¡!¿?";
    short int i = 0;
    
    while(signos[i]){
        if(signos[i] == c) return true;
        i++;
    }
    
    return false;
}

/* @func: formatear un Array de carácteres removiendo los signos de puntuación
 *        presentes en él y pasando todos los carácteres a minúsculas.
 * @params: p => Array a formatear
 * @return: void */

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

/* @func: define en memoria un nuevo nodo de tipo de árbol.
 * @params: */

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

/* @func: Remueve de memoria todos los elementos de una lista enlazada a partir
 *        de un elemento de inicio.
 * @params: ls => Puntero a la palabra de la lista desde la cual remover
 *                elementos de la lista.
 * @return: void */

void eliminarLista(palabra* ls)
{
    if(ls -> sig != NULL){
         eliminarLista(ls -> sig);
         delete ls;
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

/* @func: Recorre un árbol y imprime la lista de palabras de cada nodo.
 * @params: abo => árbol binario de tipo arbol a recorrer
 * @return: void */

void imprimirListasArbol(arbol* abo)
{
    if(abo -> ri != NULL) imprimirListasArbol(abo -> ri);
    if(abo -> rd != NULL) imprimirListasArbol(abo -> rd);
    
    imprimirListaPalabras(abo -> l);
}


//Función que retorna una lista enlazada con las palabras menos frecuentes
//@return: puntero al primer elemento de la lista

palabra* menosFrecuentes(arbol* abo, palabra* minFreq)
{
    if(abo -> ri != NULL) menosFrecuentes(abo -> ri, minFreq);
    if(abo -> rd != NULL) menosFrecuentes(abo -> rd, minFreq);
    
    palabra* ls =  abo -> l;
    while(ls != NULL){
        if((ls -> freq) < (minFreq -> freq)){
            if(minFreq -> sig != NULL) eliminarLista(minFreq -> sig);
            minFreq -> freq = ls -> freq;
            minFreq -> sig = nuevoNodoPalabra(ls -> pal);
            minFreq -> sig -> freq = minFreq -> freq;
        }
        else if((ls -> freq) == (minFreq -> freq)){
            palabra* inicio = minFreq;
            while(minFreq -> sig != NULL) minFreq = minFreq -> sig;
            minFreq -> sig = nuevoNodoPalabra(ls -> pal);
            minFreq -> sig -> freq = minFreq -> freq;
            minFreq = inicio;
        }
        ls = ls -> sig;
    }
    
    return minFreq;
}

//Función que retorna una lista enlazada con las palabras más frecuentes
//@return: puntero al primer elemento de la lista

palabra* masFrecuentes(arbol* abo, palabra* maxFreq)
{
    if(abo -> ri != NULL)  masFrecuentes(abo -> ri, maxFreq);
    if(abo -> rd != NULL)  masFrecuentes(abo -> rd, maxFreq);

    palabra* ls =  abo -> l;
    while(ls != NULL){
        if((ls -> freq) > (maxFreq -> freq)){
            if(maxFreq -> sig != NULL) eliminarLista(maxFreq -> sig);
            maxFreq -> freq = ls -> freq;
            maxFreq -> sig = nuevoNodoPalabra(ls -> pal);
            maxFreq -> sig -> freq = maxFreq -> freq;
        }
        else if((ls -> freq) == (maxFreq -> freq)){
            palabra* inicio = maxFreq;
            while(maxFreq -> sig != NULL) maxFreq = maxFreq -> sig;
            maxFreq -> sig = nuevoNodoPalabra(ls -> pal);
            maxFreq -> sig -> freq = maxFreq -> freq;
            maxFreq = inicio;
        }
        ls = ls -> sig;
    }
    return maxFreq;
}

//Función que retorna una lista enlazada con las palabras de menor longitud
//@return: puntero al primer elemento de la lista

palabra* menosCaracteres(arbol* abo, palabra* minLong)
{
    if(abo -> ri != NULL)  menosCaracteres(abo -> ri, minLong);
    if(abo -> rd != NULL)  menosCaracteres(abo -> rd, minLong);

    palabra* ls =  abo -> l;
    while(ls != NULL){
        if(strlen(ls -> pal) < unsigned(minLong -> freq)){
            if(minLong -> sig != NULL) eliminarLista(minLong -> sig);
            minLong -> freq = strlen(ls -> pal);
            minLong -> sig = nuevoNodoPalabra(ls -> pal);
            minLong -> sig -> freq = ls -> freq;
        }
        else if(strlen(ls -> pal) == unsigned(minLong -> freq)){
            palabra* inicio = minLong;
            while(minLong -> sig != NULL) minLong = minLong -> sig;
            minLong -> sig = nuevoNodoPalabra(ls -> pal);
            minLong -> sig -> freq = ls -> freq;
            minLong = inicio;
        }
        ls = ls -> sig;
    }

    return minLong;
}

//Función que retorna una lista enlazada con las palabras de menor longitud
//@return: puntero al primer elemento de la lista

palabra* masCaracteres(arbol* abo, palabra* maxLong)
{
    if(abo -> ri != NULL)  masCaracteres(abo -> ri, maxLong);
    if(abo -> rd != NULL)  masCaracteres(abo -> rd, maxLong);
    
    palabra* ls =  abo -> l;
    while(ls != NULL){
        if(strlen(ls -> pal) > unsigned(maxLong -> freq)){
            if(maxLong -> sig != NULL) eliminarLista(maxLong -> sig);
            maxLong -> freq = strlen(ls -> pal);
            maxLong -> sig = nuevoNodoPalabra(ls -> pal);
            maxLong -> sig -> freq = ls -> freq;
        }
        else if(strlen(ls -> pal) == unsigned(maxLong -> freq)){
            palabra* inicio = maxLong;
            while(maxLong -> sig != NULL) maxLong = maxLong -> sig;
            maxLong -> sig = nuevoNodoPalabra(ls -> pal);
            maxLong -> sig -> freq = ls -> freq;
            maxLong = inicio;
        }
        ls = ls -> sig;
    }
    
    return maxLong;
}

//Función que elimina por completo un arbol

void eliminarArbol(arbol* a)
{
    if(a -> ri != NULL){ eliminarArbol(a -> ri);}
    if(a -> rd != NULL){ eliminarArbol(a -> rd);}
    
    eliminarLista(a -> l);
    delete a;
}

//Función que elimina un árbol actual y construye uno nuevo basado en una
//nueva ruta de un archivo.

arbol* nuevoArbolArchivo(arbol* abo)
{
    eliminarArbol(abo);
    
    char ruta[30];
    
    cout << "Ingrese la ruta del archivo que desea analizar:                   \n"
         << ">>> ";

    cin >> ruta;
    
    cout << "\n\n";
    
    arbol* nuevo_arbol = ConstruirABO(ruta);
    
    return nuevo_arbol;
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
        
        if(opcion == 'x'){
            eliminarArbol(abo);
            break;
        }
        else if(opcion == 'n'){
            abo = nuevoArbolArchivo(abo);
            continue; 
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
            
            max = masCaracteres(abo, max);
            min = menosCaracteres(abo, min);
            
            cout << "\n"
                 << "Las palabras más largas del documento tienen " << max -> freq << " caracter(es).\n\n"
                 << " palabras más largas       ||   frecuencia \n"
                 << "============================================\n";
                 
            imprimirListaPalabras(max -> sig);
            
            cout << "\n\n"
                 << "Las palabras más cortas del documento tienen " << min -> freq << " caracter(es).\n\n"
                 << " palabras más cortas        ||  frecuencia  \n"
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
           abo = nuevoArbolArchivo(abo);
       }
       else{
           eliminarArbol(abo);
           break;
       }
       
       
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

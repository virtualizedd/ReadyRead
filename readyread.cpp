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

//Estructura que almacena un nodo del arbol y propiedades tales cómo
//la longitud de la lista de palabras y el nivel en que se encuentra

struct abo_nodo
{
    arbol* nodo = NULL;
    arbol* padre = NULL;
    int len;
    int nivel;
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

/* @func: crea un nuevo nodo de tipo árbol y lo inserta en un árbol binario
 *        ordenado.
 * @params:
 * @return: un puntero al nodo raíz del árbol */

arbol* insert_ABO(arbol* abo, char p[30])
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
    if(ls -> sig != NULL) eliminarLista(ls -> sig);
    delete ls;
}

/* @func: construye un árbol binario ordenado (ABO) a partir de las palabras
 *        presentes en un archivo de texto.
 * @params: ruta => ruta del archivo
 * @return: puntero al inicio de la lista */

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

/* @func: imprime una tabla con los valores de pal y freq de cada nodo de tipo
 *        palabra presente en una lista enlazada.
 * @params: lista => lista a recorrer.
 *          titulo_tabla => titulo a mostrar en la columna de las palabras
 * @return: void */

void imprimirListaPalabras(palabra* lista, char titulo_tabla[30])
{     
    cout << "    " << setw(15) << setfill(' ') << left 
         << titulo_tabla << "   ||  frecuencia   \n"
         << "============================================\n";
         
    while(lista  != NULL){
        
        cout << " " << setw(30) << setfill(' ') << left 
             <<  lista -> pal << "     " << lista -> freq << "\n"
             << "--------------------------------------------\n";
        lista = lista -> sig;
    }
    
    cout << "\n\n";
}

/* @func: Recorre un árbol y imprime la lista de palabras de cada nodo en 
 *        orden alfabético.
 * @params: abo => árbol binario de tipo arbol a recorrer
 * @return: void */

void imprimirListasArbol(arbol* abo)
{
    if(abo -> ri != NULL) imprimirListasArbol(abo -> ri);
    
    char t[30] = "";
    t[0] = toupper(abo -> Letra);
    imprimirListaPalabras(abo -> l, t);
    
    if(abo -> rd != NULL) imprimirListasArbol(abo -> rd);
}


/* @func: construye una lista enlazada con las palabras menos frecuentes
 *        incluidas en las listas de los nodos de un árbol
 * @params: abo => árbol a recorrer
 *          minFreq => un puntero a un nodo de tipo palabra inicial que irá
 *                     recolectando las palabras menos frecuentes
 * @return: un puntero al primer elemento de la lista construida */

palabra* menosFrecuentes(arbol* abo, palabra* minFreq)
{
    if(abo -> ri != NULL) menosFrecuentes(abo -> ri, minFreq);
    
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
    
    if(abo -> rd != NULL) menosFrecuentes(abo -> rd, minFreq);
    
    return minFreq;
}

/* @func: construye una lista enlazada con las palabras más frecuentes
 *        incluidas en las listas de los nodos de un árbol
 * @params: abo => árbol a recorrer
 *          maxFreq => un puntero a un nodo de tipo palabra inicial que irá
 *                     recolectando las palabras más frecuentes
 * @return: un puntero al primer elemento de la lista construida */

palabra* masFrecuentes(arbol* abo, palabra* maxFreq)
{
    if(abo -> ri != NULL)  masFrecuentes(abo -> ri, maxFreq);
    
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
    
    if(abo -> rd != NULL)  masFrecuentes(abo -> rd, maxFreq);
    
    return maxFreq;
}

/* @func: construye una lista enlazada con las palabras más cortas
 *        incluidas en las listas de los nodos de un árbol
 * @params: abo => árbol a recorrer
 *          minLong => un puntero a un nodo de tipo palabra inicial que irá
 *                     recolectando las palabras con menos carácteres
 * @return: un puntero al primer elemento de la lista construida(nodo inicial) 
 */

palabra* menosCaracteres(arbol* abo, palabra* minLong)
{
    if(abo -> ri != NULL)  menosCaracteres(abo -> ri, minLong);

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
    
    if(abo -> rd != NULL)  menosCaracteres(abo -> rd, minLong);
    
    return minLong;
}

/* @func: construye una lista enlazada con las palabras más largas
 *        incluidas en las listas de los nodos de un árbol
 * @params: abo => árbol a recorrer
 *          minLong => un puntero a un nodo de tipo palabra inicial que irá
 *                     recolectando las palabras con más carácteres
 * @return: un puntero al primer elemento de la lista construida(nodo inicial) 
 */

palabra* masCaracteres(arbol* abo, palabra* maxLong)
{
    if(abo -> ri != NULL)  masCaracteres(abo -> ri, maxLong);
    
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
    
    if(abo -> rd != NULL)  masCaracteres(abo -> rd, maxLong);
    
    return maxLong;
}

/* @func: elimina de memoria un árbol binario completo
 * @params: a => arbol a eliminar
 * @return: void */

void eliminarArbol(arbol* a)
{
    if(a -> ri != NULL){ eliminarArbol(a -> ri);}
    if(a -> rd != NULL){ eliminarArbol(a -> rd);}
    
    eliminarLista(a -> l);
    delete a;
}

/* @func: Recorre una lista enlazada y calcula el total de palabras
 * @params: l => lista a recorrer
 * @return: suma total de todas las frecuencias */

int sumaFreq(palabra* l)
{
    int sum = 0;
    while(l != NULL){
        sum++;
        l = l -> sig;
    }
    return sum;
}

/* @func: crea un nuevo objeto de tipo abo_nodo
 * @params:
 * @return: puntero al objeto creado */
 
abo_nodo* nuevoABONodo()
{
    abo_nodo* an = new abo_nodo;
    an -> nivel = 0;
    an -> len = 0;
    
    return an;
}
 
/* @func: Recorre un árbol y determina cuál nodo tiene la lista con menos palabras.
 *        Si hay más de un nodo con el mismo número de palabras, considera aquél
 *        en un nivel más alto.
 * @params: abo => árbol a recorrer
 * @return: puntero al nodo con más palabras */

abo_nodo* nodoMenosPalabras(arbol* abo, abo_nodo* an = nuevoABONodo(), int nivel = 0)
{   
    nivel++;
    
    if(abo -> ri != NULL) an = nodoMenosPalabras(abo -> ri, an, nivel);
    if(abo -> rd != NULL) an = nodoMenosPalabras(abo -> rd, an, nivel);
    
    int n = sumaFreq(abo -> l);
    
    if((n <= an -> len or an -> len == 0) and nivel > (an -> nivel)){
        an -> nodo = abo;
        an -> nivel = nivel;
        an -> len = n;
    }
    
    return an;
}

/* @func: elimina un nodo de un árbol binario y lo reestructura
 * @params: nodo => puntero al nodo que se desea eliminar
 * @return: void */
 
void insertNodoABO(arbol* abo, arbol* nodo)
{
    if(abo -> Letra > nodo -> Letra){
        if(abo -> rd == NULL) abo -> rd = nodo;
        else insertNodoABO(abo -> rd, nodo);
    }
    else if(abo -> Letra < nodo -> Letra){
        if(abo -> ri == NULL) abo -> ri = nodo;
        else insertNodoABO(abo -> ri, nodo);
    }
}

void punteroPadreNULL(arbol* abo, abo_nodo* an)
{
    if(abo -> ri == an -> nodo) abo -> ri = NULL;
    if(abo -> rd == an -> nodo) abo -> rd = NULL;
    
    if(abo -> ri != NULL) punteroPadreNULL(abo -> ri, an);
    if(abo -> rd != NULL) punteroPadreNULL(abo -> rd, an);
}

void  eliminarABONodo(arbol* abo, abo_nodo* an)
{
    arbol *izq = NULL;
    arbol *der = NULL;
    
    if(an -> nodo -> ri != NULL) izq = an -> nodo -> ri;
    if(an -> nodo -> rd != NULL) der = an -> nodo -> rd;
    
    punteroPadreNULL(abo, an);
    eliminarLista(an -> nodo -> l);
    delete (an -> nodo);
    
    if(izq != NULL) insertNodoABO(abo, izq);
    if(der != NULL) insertNodoABO(abo, der);
    
    delete an;
}

/* @func: elimina una palabra de una lista enlazada de palabras
 * @params: ls => lista de la cuál remover la palabra
 *          p => palabra a eliminar
 * @return: un puntero al inicio de la lista o un puntero a NULL si la palabra
 *          no es encontrada */

palabra* eliminarPalabraLista(palabra* ls, char p[30])
{
    palabra* n = NULL;
    palabra* inicio = ls;
    if(strcmp(ls -> pal, p) == 0){
        if(ls -> sig != NULL)  n = ls -> sig;
        delete ls;
        return n;
    }
    else{
        if(ls -> sig != NULL){
            n = ls;
            ls = ls -> sig;
        }
        else return n;
        
        while(ls != NULL){
            if(strcmp(ls -> pal, p) == 0){
                n -> sig =  ls -> sig;
                delete ls;
                return inicio;
            }
            n = ls;
            ls = ls -> sig;
        }
        return (n -> sig);
    }
}

/* @func: elimina una palabra de un árbol binario
 * @params: abo => árbol al cual eliminar la palabra
 *          p => palabra a eliminar
 * @return: void*/

palabra* eliminarPalabraABO(arbol* abo, char p[30])
{
    if(p[0] == abo -> Letra){
        palabra* r = eliminarPalabraLista(abo -> l, p);
        abo -> l = r;
        return r;
    }
    else{
        palabra* r;
        if(abo -> ri != NULL)r = eliminarPalabraABO(abo -> ri, p);
        if(abo -> rd != NULL)r = eliminarPalabraABO(abo -> rd, p);
        return r;
    }
}

/* @func: elimina un árbol actual y construye uno nuevo basado en una
 *         nueva ruta de archivo.
 * @params: abo => árbol a destruir 
 * @return: un puntero al nodo raíz del nuevo árbol construido */

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

/* @func: interfaz para acceder a las distintas funcionalidades que ofrece
 *        el programa.
 * @params: abo => árbol sobre el que se trabajará inicialmente.
 * @return: void */

void menuOpciones(arbol* abo)
{
    while(true){
        cout << "¿Qué deseas hacer?\n\n"
             << "   [1] Listar todas las palabras presentes en el archivo\n"
             << "       junto a su frecuencia de aparición\n"
             << "   [2] Saber cuáles son las palabras más y menos frecuentes                  \n"
             << "   [3] Saber cuáles son las palabras con mayor y menor número de carácteres  \n"
             << "   [4] Elimina del ABO generado el nodo con la lista más breve de palabras   \n"
             << "   [5] Elimina una palabra del ABO                                           \n"
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
            imprimirListasArbol(abo);
            
        }
        else if(opcion == '2'){
            char inicio[30] = "_init_";
            palabra* max = nuevoNodoPalabra(inicio);
            palabra* min = nuevoNodoPalabra(inicio);
            max = masFrecuentes(abo, max);
            min = menosFrecuentes(abo, min);
            
            char t1[30] = "palabras más repetidas";     
            imprimirListaPalabras(max -> sig, t1);
            
            char t2[30] = "palabras menos repetidas";     
            imprimirListaPalabras(min -> sig, t2);
            
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
                 << "Las palabras más largas del documento tienen " 
                 << max -> freq << " caracter(es).\n\n";
            
            char t1[30] = "palabras más largas";
            imprimirListaPalabras(max -> sig, t1);
            
            cout << "Las palabras más cortas del documento tienen " 
                 << min -> freq << " caracter(es).\n\n";
                 
            char t2[30] = "palabras más cortas";
            imprimirListaPalabras(min -> sig, t2);
            
            eliminarLista(max);
            eliminarLista(min);
        }
        else if(opcion == '4'){
            abo_nodo* nodo_eliminar = nodoMenosPalabras(abo);
            
            cout << " El nodo con la lista más breve de palabras corresponde al nodo con\n el caŕacter " 
                 << nodo_eliminar -> nodo -> Letra << ". ";
            
            eliminarABONodo(abo, nodo_eliminar);
            
            cout << " El nodo ha sido eliminado correctamente.\n\n";
        }
        else if(opcion == '5'){
            char p[30];
            cout << "¿Qué palabra desea eliminar?\n>>> ";
            cin >> p;
            formatearPalabra(p);
            palabra* r = eliminarPalabraABO(abo, p);
            if(r == NULL) cout << "\n\nLa palabra " << p << " no se encontraba en la lista\n";
            else cout << "\nLa palabra se ha eliminado correctamente.\n\n";
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

/*****************************************************************************/
         
/************************ MAIN ***********************************************/

int main()
{
    
    cout << "|||||| ReadyRead v.0.0.1 - inspector de archivos |||||||||||||||||\n"
         << "------------------------------------------------------------------\n"
         << "                                                                  \n"
         << "Ingrese la ruta del archivo que desea analizar:                   \n"
         << ">>> ";
    
    char ruta_archivo[255];
    cin >> ruta_archivo;
    
    cout << "\n\n";
    
    arbol* arbol_palabras = ConstruirABO(ruta_archivo);
    
    menuOpciones(arbol_palabras);
    
    return 0;
}

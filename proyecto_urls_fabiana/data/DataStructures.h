#include <iostream>
#include <string>
#include <cmath>     // Esta libreria la traemos únicamente para poder hacer sqrt()
#include <algorithm> // Esta libreria, igualmente, la traemos para usar max() y min()

using namespace std;


enum class TipoURL { Benigna = 0, Phishing = 1, Maleware = 2, Defacement = 3, Nuevo =4};
// 1. CLASE DE URL ÚNICA 

// En esta clase se analiza el constructor general para las URLS. Es importante tener en cuenta que se replica el modelo para conseguir un análisis completo de las URLS como objetos.
class caracteristicas_URL {
    private:
        TipoURL tipo;
        string url;
        int url_length; 
        int cant_dot;
        int cant_underscore;
        int cant_hyphen;
        int cant_query; 
    
    public:
        // CONSTRUCTOR (función) CARACTERISTICAS 
        caracteristicas_URL(string url, int url_length, int cant_dot, int cant_underscore, int cant_hyphen, int cant_query) {
            this-> tipo = 4;
            this-> url = url;                       
            this-> url_length = url_length ;
            this-> cant_dot = cant_dot ;
            this-> cant_underscore = cant_underscore ;
            this-> cant_hyphen = cant_hyphen ;      
            this-> cant_query = cant_query;
        }
        
        // DESTRUCTOR
        ~caracteristicas_URL() {};
                
        // GETTERS 
        TipoURL get_tipo() {
            return tipo;
        }
        string get_url() { 
            return url; 
        }
        int get_url_length() { 
            return url_length; 
        }
        int get_cant_dot() { 
            return cant_dot; 
        }
        int get_cant_underscore() {
            return cant_underscore; 
        }
        int get_cant_hyphen() { 
            return cant_hyphen; 
        }
        int get_cant_query() { 
            return cant_query; 
        }
        
        // SETTERS 
        void set_tipo(TipoURL tipo){
            this-> tipo = tipo;
        }
        void set_url(string url) { 
            this-> url = url;  // this-> para asignar un puntero en un dirección de memoria
        }
        void set_url_length(int url_length) { 
            this-> url_length = url_length; 
        }
        void set_cant_dot(int cant_dot) { 
            this-> cant_dot = cant_dot; 
        }
        void set_cant_underscore(int cant_underscore) { 
            this-> cant_underscore = cant_underscore; 
        }
        void set_cant_hyphen(int cant_hyphen) { 
            this-> cant_hyphen = cant_hyphen; 
        }
        void set_cant_query(int cant_query) { 
            this-> cant_query = cant_query; 
        }
};


// 2. FUNCIÓN PARA ANALIZAR UNA SOLA URL

caracteristicas_URL analizar_una_url(string url_ingresada) {
    int dots = 0, underscores = 0, hyphens = 0, queries = 0;
    bool tiene_query = false;
    
    // Sacamos la longitud primero con .length()
    int length = url_ingresada.length();

    // Este ciclo recorre el string de la URL. Se deben almacenar como tal para hacer el análisis
    for (int i = 0; i < length; i++) {
        char c = url_ingresada[i];
        
        if (c == '.') {
            dots++;
        }
        else if (c == '_') {
            underscores++;
        }
        else if (c == '-') { 
            hyphens++;
        }
        
        // Lógica para los componentes de consulta (Query), acá se analiza qué hay después del ? y cómo el & está separando subdominios
        if (c == '?') { 
        tiene_query = true;
        }
        if (tiene_query && c == '&') {
            queries++;
        }
    }
    
    if (tiene_query) {
        queries++; 
    }

    // Después del análisis y con los contadores actualizados, toca mostrarlos según lo que se obtuvo
    return caracteristicas_URL(url_ingresada, length, dots, underscores, hyphens, queries);
}


// 3. BASE DE DATOS EN MEMORIA
class BaseDatosURL {
    public:
        double distancia(const RegistroURL& a, const RegistroURL& b) const;
    private:
        vector<RegistroURL> urls;
        void agregar(const RegistroURL& r);
        ResultadoAnalisis analizar(const string& url) const;
        map<TipoURL, int> distribucionPorTipo() const;
        int total() const;
        static RegistroURL extraerParametros(const std::string& url);
    };


// 5. FUNCIONES DE ESTADÍSTICA (Medidas de Tendencia Central y de Dispersión)
// Importantísimo acá: Estas funciones son para las "columnas" de las características ya creadas. Se almacenan como una lista []

// PROMEDIO 
float calcular_promedio(int lista[], int tamano) {
    float suma = 0;
    for(int i = 0; i < tamano; i++) {
        suma += lista[i];
    }
    return suma / tamano;
}

// ORDENAR LISTA (Usando la lógica de buscar el mínimo para la Mediana)
void ordenar_lista(int lista[], int tamano) {
    for (int i = 0; i < tamano - 1; i++) {
        for (int j = i + 1; j < tamano; j++) {
            // Usamos min() para saber si el de la derecha es menor y se van cambiando las variables encontrando el mayor para ponerlo de primeras
            if (min(lista[i], lista[j]) == lista[j]) {
                int temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }
}

// MEDIANA
float calcular_mediana(int lista_original[], int tamano) {
    int copia[]; 
    for(int i = 0; i < tamano; i++) {
        copia[i] = lista_original[i];
    }
    
    ordenar_lista(copia, tamano);
    
    // Se evalúa si la mediana estará en un punto exacto (Si el tamaño es impar) o si toca hacer el promedo de los dos del centro cuando el tamaño de la lista es par
    if (tamano % 2 == 0) { 
        return (copia[tamano / 2 - 1] + copia[tamano / 2]) / 2.0;
    } else {
        return copia[tamano / 2];
    }
}

int calcular_rango(int lista[], int tamano) {
    int valor_maximo = lista[0];
    int valor_minimo = lista[0];
    
    for(int i = 1; i < tamano; i++) {
        valor_maximo = max(valor_maximo, lista[i]); // Compara y guarda el mayor
        valor_minimo = min(valor_minimo, lista[i]); // Compara y guarda el menor
    }
    return valor_maximo - valor_minimo;
}

// POTENCIA CON CICLOS PARA PODER DESPUÉS USARLA EN VARIANZA
float elevar_potencia(float base, int exponente) {
    float resultado = 1.0; 
    for (int i = 0; i < exponente; i++) {
        resultado = resultado * base;
    }
    return resultado;
}

// VARIANZA
float calcular_varianza(int lista[], int tamano, float promedio) {
    float suma_diferencias = 0;
    
    for(int i = 0; i < tamano; i++) {
        float diferencia = lista[i] - promedio;
        suma_diferencias += elevar_potencia(diferencia, 2); 
    }
    return suma_diferencias / tamano; 
}

// DESVIACIÓN ESTÁNDAR
float calcular_desviacion(float varianza) {
    return sqrt(varianza); 
}



// 5. EXTRACCIÓN Y ANÁLISIS DE BASE DE DATOS
// Esto solo es una simulación de cómo crearíamos la lista de URLs para malware y defacement, ya que se necesita sacar esa lista, luego sacar los datos, y por 
// último, utilizar las funciones de estadísitca para arrojar las caracterísiticas que necesitamos.
void simular_procesamiento_base_datos() { // Esto aplica para MALWARE.
    
    const int TAMANO_BD = 5;
    string urls_crudas_malware[TAMANO_BD] = { // Son 5 URLS las que pertenecen a la lista general
        "http://www.824555.com/app/member/SportOption.php?uid=guest&langx=gb",  // Estos son algunos ejemplos tomados de la base de datos real de Malware, toca así con defacement y hacer todo el proceso
        "http://9779.info/%E5%84%BF%E7%AB%A5%E7%AB%8B%E4%BD%93%E7%BA%B8%E8%B4%B4%E7%94%BB/",
        "http://9779.info/%E6%A0%91%E5%8F%B6%E7%B2%98%E8%B4%B4%E7%94%BB/",
        "http://www.wu8188.com/cl/tpl/five-star/ver1/css/five-star.css?v=ver15.32",
        "proplast.co.nz"
    };

    // Creación de las listas. Longitud de primeras.
    int columna_longitudes[TAMANO_BD]; 
    int columna_puntos[TAMANO_BD];
    int columna_underscores[TAMANO_BD];
    int columna_guiones[TAMANO_BD];
    int columna_queries[TAMANO_BD];

    cout << " PROCESANDO BASE DE DATOS SIMULADA " << endl;
    for (int i = 0; i < TAMANO_BD; i++) {
        caracteristicas_URL obj_procesado = analizar_una_url(urls_crudas_malware[i]);
        
        // Asignación de listas, importante tener en cuenta que acá las posiciones "i" se refieren a URLS únicas
        columna_longitudes[i] = get_url_length(obj_procesado); 
        columna_puntos[i]  = get_cant_dot(obj_procesado);
        columna_underscores[i] = get_cant_underscore(obj_procesado);
        columna_guiones[i] = get_cant_hyphen(obj_procesado);
        columna_queries[i] = get_cant_query(obj_procesado);
        
        cout << "URL [" << i << "] procesada." << endl;
    }

    // Aplicamos las estadísticas a una de las listas (ejemplo: Longitudes), esto habría que hacerlo varias veces para poder lograr hacer el análisis con todo
    cout << " ESTADISTICAS DE LA COLUMNA: url_length (Longitud) " << endl;
    
    float prom = calcular_promedio(columna_longitudes, TAMANO_BD); // Tamaño BD es el tamaño de la columna que encontramos para poder ingresarselo a la lista
    float med = calcular_mediana(columna_longitudes, TAMANO_BD);
    int rango = calcular_rango(columna_longitudes, TAMANO_BD);
    float var = calcular_varianza(columna_longitudes, TAMANO_BD, prom);
    float desv = calcular_desviacion(var);
    
    cout << "Media: " << prom << endl;
    cout << "Mediana: " << med << endl;
    cout << "Rango: " << rango << endl;
    cout << "Varianza: " << var << endl;
    cout << "Desv. Estandar: " << desv << endl;
}
    

//int main() {
//    cout << "SISTEMA DE DETECCION DE URLS (BOSQUEJO Y SIMULACION PEQUENA)" << endl;
//    
//    // PRUEBA 1: Analisis de URL individual
//    cout << " ANALISIS INDIVIDUAL " << endl;
//    string mi_url_para_analizar = "myspace.com/video/vid/30602581"; // Esta URL es benigna y es sacada también de la base de datos
//    caracteristicas_URL obj_url = analizar_una_url(mi_url_para_analizar);
//    
//    cout << "URL Analizada: " << get_url(obj_url) << endl;
//    cout << "Longitud (url_length): " << get_url_length(obj_url) << endl; 
//    cout << "Puntos (cant_dot): " << get_cant_dot(obj_url) << endl;
//    cout << "Underscores (cant_underscore): " << get_cant_underscore(obj_url) << endl;
//    cout << "Guiones (cant_hyphen): " << get_cant_hyphen(obj_url) << endl;
//    cout << "Queries (cant_query): " << get_cant_query(obj_url) << endl;
//    
//    // PRUEBA 2: Simulación del procesamiento de la base de datos. 
//    // Claramente esto es a pequeña escala pero el código muestra el seguimiento general para después replicarlo
//    simular_procesamiento_base_datos();
//    
//    return 0;
//}


        

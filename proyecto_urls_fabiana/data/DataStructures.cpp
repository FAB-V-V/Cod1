#include "DataStructures.h"

// caracteristicas_URL

caracteristicas_URL::caracteristicas_URL(string url, int url_length, int cant_dot, int cant_underscore, int cant_hyphen, int cant_query) {
    this->tipo = TipoURL::Benigna;    //Por defecto inicia en benigna
    this->url = url;
    this->url_length = url_length;
    this->cant_dot = cant_dot;
    this->cant_underscore= cant_underscore;
    this->cant_hyphen = cant_hyphen;
    this->cant_query = cant_query;
}

TipoURL caracteristicas_URL::get_tipo(){ 
    return tipo; 
}
string caracteristicas_URL::get_url(){
    return url; 
}
int caracteristicas_URL::get_url_length(){ 
    return url_length; 
}
int caracteristicas_URL::get_cant_dot(){
    return cant_dot; 
}
int caracteristicas_URL::get_cant_underscore(){
    return cant_underscore; 
}
int caracteristicas_URL::get_cant_hyphen(){
    return cant_hyphen; 
}
int caracteristicas_URL::get_cant_query(){
    return cant_query;
}

void caracteristicas_URL::set_tipo(TipoURL t){
    tipo = t;
}
void caracteristicas_URL::set_url(string u){
    url = u;
}
void caracteristicas_URL::set_url_length(int v){
    url_length = v;
}
void caracteristicas_URL::set_cant_dot(int v){
    cant_dot = v;
}
void caracteristicas_URL::set_cant_underscore(int v){
    cant_underscore = v;
}
void caracteristicas_URL::set_cant_hyphen(int v){
    cant_hyphen = v;
}
void caracteristicas_URL::set_cant_query(int v){
    cant_query = v;
}


// FUNCIÓN PARA ANALIZAR UNA SOLA URL
caracteristicas_URL analizar_una_url(string url_ingresada) {
    int dots = 0, underscores = 0, hyphens = 0, queries = 0;
    bool tiene_query = false;
    int length = url_ingresada.length();

    for(int i = 0; i < length; i++) {
        char c = url_ingresada[i];

        if(c == '.'){
            dots++;
        }
        else if(c == '_'){
           underscores++;
        }
        else if(c == '-'){
            hyphens++;
        }

        // Lógica para los componentes de consulta (Query)
        if(c == '?'){
            tiene_query = true;
        }
        if(tiene_query && c == '&'){
            queries++;
        }
    }
    if(tiene_query){
        queries++;
    }

    return caracteristicas_URL(url_ingresada, length, dots, underscores, hyphens, queries);
}


// FUNCIONES DE ESTADÍSTICA
float calcular_promedio(int lista[], int tamano) {
    float suma = 0;
    for (int i = 0; i < tamano; i++) suma += lista[i];
    return suma / tamano;
}

void ordenar_lista(int lista[], int tamano) {
    for (int i = 0; i < tamano - 1; i++) {
        for (int j = i + 1; j < tamano; j++) {
            if (lista[j] < lista[i]) {
                int temp = lista[i];
                lista[i] = lista[j];
                lista[j] = temp;
            }
        }
    }
}

float calcular_mediana(int lista_original[], int tamano) {
    // Usamos vector para evitar el VLA inválido "int copia[]" del código original
    vector<int> copia(lista_original, lista_original + tamano);
    ordenar_lista(copia.data(), tamano);

    if (tamano % 2 == 0)
        return (copia[tamano / 2 - 1] + copia[tamano / 2]) / 2.0f;
    return copia[tamano / 2];
}

int calcular_rango(int lista[], int tamano) {
    int maximo = lista[0], minimo = lista[0];
    for (int i = 1; i < tamano; i++) {
        maximo = max(maximo, lista[i]);
        minimo = min(minimo, lista[i]);
    }
    return maximo - minimo;
}

float elevar_potencia(float base, int exponente) {
    float resultado = 1.0f;
    for (int i = 0; i < exponente; i++) resultado *= base;
    return resultado;
}

float calcular_varianza(int lista[], int tamano, float promedio) {
    float suma = 0;
    for (int i = 0; i < tamano; i++) {
        float dif = lista[i] - promedio;
        suma += elevar_potencia(dif, 2);
    }
    return suma / tamano;
}

float calcular_desviacion(float varianza) {
    return sqrt(varianza);
}


// BaseDatosURL 

RegistroURL BaseDatosURL::extraerParametros(const string& url) {
    RegistroURL r;
    r.tipo = TipoURL::Nuevo;
    r.urlLength = (int)url.size();
    r.dots = 0;
    r.underscores = 0;
    r.hyphens = 0;
    r.queries = 0;

    bool tiene_query = false;
    for (char c : url) {
        if (c == '.'){
            r.dots++;
        }
        else if (c == '_'){
            r.underscores++;
        }
        else if (c == '-'){
            r.hyphens++;
        }

        if (c == '?'){
            tiene_query = true;
        }
        if (tiene_query && c == '&'){
            r.queries++;
        }
    }
    if (tiene_query){
        r.queries++;
    }

    r.subdomainLevels = r.dots; // aproximación: niveles de dominio ≈ cantidad de puntos

    return r;
}

double BaseDatosURL::distancia(const RegistroURL& a, const RegistroURL& b) const {
    // distancia euclidiana normalizada: cada diferencia se divide por el máximo de su
    // característica, de modo que todas pesen entre 0 y 1 (urlLength deja de dominar).
    auto term = [](int x, int y, int m) {
        double dif = double(x - y) / (m > 0 ? m : 1);
        return dif * dif;
    };
    double d = 0;
    d += term(a.urlLength,    b.urlLength,    maxUrlLength);
    d += term(a.dots,         b.dots,         maxDots);
    d += term(a.underscores,  b.underscores,  maxUnderscores);
    d += term(a.hyphens,      b.hyphens,      maxHyphens);
    d += term(a.queries,      b.queries,      maxQueries);
    return sqrt(d);
}

void BaseDatosURL::agregar(const RegistroURL& r) {
    urls.push_back(r);
    // actualizamos los máximos para la normalización de la distancia
    maxUrlLength   = max(maxUrlLength,   r.urlLength);
    maxDots        = max(maxDots,        r.dots);
    maxUnderscores = max(maxUnderscores, r.underscores);
    maxHyphens     = max(maxHyphens,     r.hyphens);
    maxQueries     = max(maxQueries,     r.queries);
}

int BaseDatosURL::total() const {
    return (int)urls.size();
}

map<TipoURL, int> BaseDatosURL::distribucionPorTipo() const {
    map<TipoURL, int> dist;
    for (const auto& u : urls) {
        dist[u.tipo]++;
    }
    return dist;
}

ResultadoAnalisis BaseDatosURL::analizar(const string& url) const {
    RegistroURL consulta = extraerParametros(url);

    // K-NN con k=5
    const int K = 5;
    vector<pair<double, TipoURL>> distancias;
    distancias.reserve(urls.size());
    for (const auto& r : urls){
        distancias.push_back({distancia(consulta, r), r.tipo});
    }
    sort(distancias.begin(), distancias.end());

    map<TipoURL, int> votos;
    int k = min(K, (int)distancias.size());
    for (int i = 0; i < k; i++){
        votos[distancias[i].second]++;
    }

    TipoURL ganador = TipoURL::Benigna;
    int maxVotos = 0;
    for (auto& [tipo, cnt] : votos){
        if (cnt > maxVotos){
            maxVotos = cnt; ganador = tipo;
        }
    }

    ResultadoAnalisis res;
    res.clasificacion = ganador;
    res.confianza     = (float)maxVotos / k * 100.0f;
    res.urlLength     = consulta.urlLength;
    res.dots          = consulta.dots;
    res.underscores   = consulta.underscores;
    res.hyphens       = consulta.hyphens;
    res.queries       = consulta.queries;
    res.distribucion  = votos; // distribución de los K vecinos más cercanos
    return res;
}


// EXTRACCIÓN Y ANÁLISIS DE BASE DE DATOS (simulación pequeña)

void simular_procesamiento_base_datos() {
    const int TAMANO_BD = 5;
    string urls_crudas_malware[TAMANO_BD] = {
        "http://www.824555.com/app/member/SportOption.php?uid=guest&langx=gb",
        "http://9779.info/%E5%84%BF%E7%AB%A5%E7%AB%8B%E4%BD%93%E7%BA%B8%E8%B4%B4%E7%94%BB/",
        "http://9779.info/%E6%A0%91%E5%8F%B6%E7%B2%98%E8%B4%B4%E7%94%BB/",
        "http://www.wu8188.com/cl/tpl/five-star/ver1/css/five-star.css?v=ver15.32",
        "proplast.co.nz"
    };

    int columna_longitudes[TAMANO_BD];
    int columna_puntos[TAMANO_BD];
    int columna_underscores[TAMANO_BD];
    int columna_guiones[TAMANO_BD];
    int columna_queries[TAMANO_BD];

    cout << " PROCESANDO BASE DE DATOS SIMULADA " << endl;
    for (int i = 0; i < TAMANO_BD; i++) {
        // FIX: se llamaba como get_url_length(obj) en vez de obj.get_url_length()
        caracteristicas_URL obj_procesado = analizar_una_url(urls_crudas_malware[i]);
        columna_longitudes[i]   = obj_procesado.get_url_length();
        columna_puntos[i]       = obj_procesado.get_cant_dot();
        columna_underscores[i]  = obj_procesado.get_cant_underscore();
        columna_guiones[i]      = obj_procesado.get_cant_hyphen();
        columna_queries[i]      = obj_procesado.get_cant_query();
        cout << "URL [" << i << "] procesada." << endl;
    }

    cout << " ESTADISTICAS DE LA COLUMNA: url_length (Longitud) " << endl;
    float prom = calcular_promedio(columna_longitudes, TAMANO_BD);
    float med  = calcular_mediana(columna_longitudes, TAMANO_BD);
    int   rang = calcular_rango(columna_longitudes, TAMANO_BD);
    float var  = calcular_varianza(columna_longitudes, TAMANO_BD, prom);
    float desv = calcular_desviacion(var);

    cout << "Media: "          << prom << endl;
    cout << "Mediana: "        << med  << endl;
    cout << "Rango: "          << rang << endl;
    cout << "Varianza: "       << var  << endl;
    cout << "Desv. Estandar: " << desv << endl;
}

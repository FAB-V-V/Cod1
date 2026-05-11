#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

enum class TipoURL { Benigna = 0, Phishing = 1, Maleware = 2, Defacement = 3, Nuevo = 4 };

// Registro completo de una URL para K-NN (9 características numéricas)
struct RegistroURL {
    TipoURL tipo;
    int subdomainLevels;
    int urlLength;
    int dots;
    int underscores;
    int hyphens;
    int queries;
    int atSigns;
    int slashes;
    int hasHttps;
};

// Resultado que se le muestra al usuario tras el análisis K-NN
struct ResultadoAnalisis {
    TipoURL clasificacion;
    float confianza;
    int urlLength;
    int dots;
    int underscores;
    int hyphens;
    int queries;
    map<TipoURL, int> distribucion; // votos de los K vecinos
};


// 1. CLASE DE URL ÚNICA
// Analiza una URL como objeto con sus 5 características básicas.
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
    caracteristicas_URL(string url, int url_length, int cant_dot, int cant_underscore, int cant_hyphen, int cant_query);
    ~caracteristicas_URL() {}

    TipoURL get_tipo();
    string  get_url();
    int get_url_length();
    int get_cant_dot();
    int get_cant_underscore();
    int get_cant_hyphen();
    int get_cant_query();

    void set_tipo(TipoURL t);
    void set_url(string u);
    void set_url_length(int v);
    void set_cant_dot(int v);
    void set_cant_underscore(int v);
    void set_cant_hyphen(int v);
    void set_cant_query(int v);
};


// 3. BASE DE DATOS EN MEMORIA (K-NN)
class BaseDatosURL {
public:
    void agregar(const RegistroURL& r);
    ResultadoAnalisis analizar(const string& url) const;
    map<TipoURL, int> distribucionPorTipo() const;
    int total() const;
    double distancia(const RegistroURL& a, const RegistroURL& b) const;

private:
    vector<RegistroURL> urls;
    static RegistroURL extraerParametros(const string& url);
};


// 2. FUNCIÓN PARA ANALIZAR UNA SOLA URL
caracteristicas_URL analizar_una_url(string url_ingresada);

// 5. FUNCIONES DE ESTADÍSTICA
float calcular_promedio(int lista[], int tamano);
void  ordenar_lista(int lista[], int tamano);
float calcular_mediana(int lista_original[], int tamano);
int   calcular_rango(int lista[], int tamano);
float elevar_potencia(float base, int exponente);
float calcular_varianza(int lista[], int tamano, float promedio);
float calcular_desviacion(float varianza);

// 5. EXTRACCIÓN Y ANÁLISIS DE BASE DE DATOS (simulación pequeña)
void simular_procesamiento_base_datos();

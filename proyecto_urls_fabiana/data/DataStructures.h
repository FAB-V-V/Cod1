#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;   // el resto del código usa string/vector/map/cout sin calificar

// Nuevo = URL aún sin clasificar (la que ingresa el usuario para analizar)
enum class TipoURL { Benigna = 0, Phishing = 1, Maleware = 2, Defacement = 3, Nuevo = 4 };

// Registro completo de una URL para K-NN (compara características numéricas)
// El orden de los campos debe coincidir con la inicialización en DataLoader.cpp:
// {tipo, subdomainLevels, urlLength, dots, underscores, hyphens, queries, atSigns, slashes, hasHttps}
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
// Representa una URL que ya está clasificada y vive en el dataset
struct ResultadoAnalisis {
    std::string url;        //la URL que se analizó
    TipoURL clasificacion;
    float confianza;
    double scoreBenigna, scorePhishing, scoreMaleware, scoreDefacement;
    int urlLength, dots, underscores, hyphens, queries;
    map<TipoURL, int> distribucion; 
};


// CLASE DE URL ÚNICA
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


// BASE DE DATOS EN MEMORIA (K-NN)
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

    // máximos por característica (se actualizan en agregar) para normalizar la distancia
    // y evitar que urlLength, al ser mucho mayor, aplaste al resto de features.
    int maxUrlLength = 1, maxDots = 1, maxUnderscores = 1, maxHyphens = 1, maxQueries = 1;
};


// FUNCIÓN PARA ANALIZAR UNA SOLA URL
caracteristicas_URL analizar_una_url(string url_ingresada);

// FUNCIONES DE ESTADÍSTICA
float calcular_promedio(int lista[], int tamano);
void  ordenar_lista(int lista[], int tamano);
float calcular_mediana(int lista_original[], int tamano);
int   calcular_rango(int lista[], int tamano);
float elevar_potencia(float base, int exponente);
float calcular_varianza(int lista[], int tamano, float promedio);
float calcular_desviacion(float varianza);

// EXTRACCIÓN Y ANÁLISIS DE BASE DE DATOS (simulación pequeña)
void simular_procesamiento_base_datos();

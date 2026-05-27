// es el unico puente entre los archivos de datos y el programa
// lee las dos bases de datos (phishing + benignas) con QFile/QTextStream y llena los vectores

#pragma once
#include "DataStructures.h"
#include <string>

class DataLoader {
public:
    // sin parametros: carga las dos bases por defecto (phishing + benignas),
    // las une en una sola base y, si ambas existen, genera el tercer archivo unido.
    static BaseDatosURL cargar();

    // une dos bases de datos (una de phishing y otra de benignas) en una sola.
    // esta base unida es la que se analiza con K-NN: equivale al archivo original
    // completo, asi que toda la logica posterior del codigo queda igual.
    // El tipo (phishing/benigna) de cada fila se toma de su columna CLASS_LABEL,
    // por lo que no importa el orden en que se pasen las rutas.
    static BaseDatosURL unir(const std::string& rutaPhishing, const std::string& rutaBenignas);

    // depura las dos bases y guarda la union en un tercer CSV (reconstruye el dataset
    // original: cabecera + filas de phishing + filas de benignas). Devuelve true si pudo.
    static bool guardarUnion(const std::string& rutaPhishing,
                             const std::string& rutaBenignas,
                             const std::string& rutaSalida);

private:
    static void cargarCSV(BaseDatosURL& db, const std::string& ruta);     // carga el CSV indicado en la base
    static void cargarDemo(BaseDatosURL& db);                             // datos de prueba minimos para que el K-NN funcione aunque no haya CSVs
};

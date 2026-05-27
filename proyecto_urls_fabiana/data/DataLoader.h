// es el unico puente entre los archivos de datos y el programa
// lee los dos Excel (64k URLs + tendencias) con QXlsx y llena los vectores

#pragma once
#include "DataStructures.h"
#include <string>

class DataLoader {
public:
    // sin parametro: carga las dos bases de datos desde rutas por defecto.
    // con parametro: carga el CSV especificado por el usuario.
    static BaseDatosURL cargar(const std::string& rutaCSV = "");
    
private:
    static void cargarCSV1(BaseDatosURL& db, const std::string& ruta);    // carga base1_features.csv  /  Benigna (Label=0) y Phishing (Label=1)
    static void cargarCSV2(BaseDatosURL& db, const std::string& ruta);    // carga base2_features.csv  /  Maleware y Defacement
    static void cargarDemo(BaseDatosURL& db);                             // datos de prueba minimos para que el K-NN funcione aunque no haya CSVs
};

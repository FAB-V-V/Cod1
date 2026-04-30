// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: DataLoader — lee los dos Excel (64k URLs + tendencias) con QXlsx y llena los vectores

#pragma once
#include "DataStructures.h"
#include <string>

class DataLoader {
public:
    static BaseDatosURL cargar(const std::string& rutaCSV = "");
private:
    static void cargarCSV(BaseDatosURL& db, const std::string& ruta);
    static void cargarDemo(BaseDatosURL& db);
};

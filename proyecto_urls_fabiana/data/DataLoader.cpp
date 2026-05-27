#include "DataLoader.h"
#include <QFile>
#include <QTextStream>
#include <QString>

BaseDatosURL DataLoader::cargar(const std::string& rutaCSV) {
    BaseDatosURL db;
    if (rutaCSV.empty())
        cargarDemo(db);
    else
        cargarCSV(db, rutaCSV);
    return db;
}

void DataLoader::cargarCSV(BaseDatosURL& db, const std::string& ruta) {
    // TODO: implementar carga desde archivo CSV
    (void)db; (void)ruta;
}

void DataLoader::cargarDemo(BaseDatosURL& db) {
    // Orden de campos: {tipo, subdomainLevels, urlLength, dots, underscores, hyphens, queries, atSigns, slashes, hasHttps}

    // URLs benignas: pocas características sospechosas
    db.agregar({TipoURL::Benigna,     2, 20, 1, 0, 0, 0, 0, 2, 1}); // google.com
    db.agregar({TipoURL::Benigna,     3, 30, 2, 0, 0, 0, 0, 3, 0}); // wikipedia.org/wiki/algo

    // URLs maliciosas: muchas características sospechosas
    // FIX: era TipoURL::Maliciosa que no existe en el enum
    db.agregar({TipoURL::Phishing,    5, 80, 3, 1, 2, 4, 0, 10, 0}); // phishing largo
    db.agregar({TipoURL::Maleware,    4, 60, 2, 1, 0, 6, 1,  8, 0}); // con @ y sin https
}

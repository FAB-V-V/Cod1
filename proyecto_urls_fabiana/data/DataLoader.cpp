#include "DataLoader.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

// Nombre del CSV de tendencias (URLs ya etiquetadas como benignas/phishing).
// PROJECT_DATA_DIR lo define CMake con la ruta absoluta de la carpeta /data.
#ifdef PROJECT_DATA_DIR
static const std::string CSV_TENDENCIAS =
    std::string(PROJECT_DATA_DIR) + "/BASE DE DATOS 1(d92e3cc325339e8e82d3f498ae69618).csv";
#else
static const std::string CSV_TENDENCIAS = "";
#endif

BaseDatosURL DataLoader::cargar(const std::string& rutaCSV) {
    BaseDatosURL db;

    // sin parámetro -> usamos el CSV de tendencias por defecto; con parámetro, el del usuario.
    std::string ruta = rutaCSV.empty() ? CSV_TENDENCIAS : rutaCSV;

    if (!ruta.empty())
        cargarCSV(db, ruta);

    // si el CSV no se pudo abrir o estaba vacío, caemos en datos demo para no quedar sin base.
    if (db.total() == 0)
        cargarDemo(db);

    return db;
}

void DataLoader::cargarCSV(BaseDatosURL& db, const std::string& ruta) {
    QFile archivo(QString::fromStdString(ruta));
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return;   // no se pudo abrir; cargar() recurrirá a los datos demo

    QTextStream in(&archivo);
    bool primeraLinea = true;

    // Índices de columna en el CSV (separador ';'):
    //  1=NumDots  2=SubdomainLevel  4=UrlLength  5=NumDash
    //  7=AtSymbol 9=NumUnderscore  11=NumQueryComponents 15=NoHttps  49=CLASS_LABEL
    while (!in.atEnd()) {
        QString linea = in.readLine();

        if (primeraLinea) {           // saltar la fila de cabecera
            primeraLinea = false;
            continue;
        }
        if (linea.trimmed().isEmpty())
            continue;

        QStringList c = linea.split(';');
        if (c.size() < 50)            // fila mal formada
            continue;

        RegistroURL r{};
        r.subdomainLevels = c[2].toInt();
        r.urlLength       = c[4].toInt();
        r.dots            = c[1].toInt();
        r.underscores     = c[9].toInt();
        r.hyphens         = c[5].toInt();
        r.queries         = c[11].toInt();
        r.atSigns         = c[7].toInt();
        r.slashes         = 0;                              // no viene como columna directa
        r.hasHttps        = (c[15].toInt() == 0) ? 1 : 0;   // NoHttps invertido

        // CLASS_LABEL: 1 = Phishing, 0 = Benigna
        r.tipo = (c[49].toInt() == 1) ? TipoURL::Phishing : TipoURL::Benigna;

        db.agregar(r);
    }
    archivo.close();
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

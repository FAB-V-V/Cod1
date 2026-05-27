#include "DataLoader.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

// Rutas por defecto de las dos bases de datos (ya separadas por tipo).
// PROJECT_DATA_DIR lo define CMake con la ruta absoluta de la carpeta /data.
#ifdef PROJECT_DATA_DIR
static const std::string CSV_PHISHING = std::string(PROJECT_DATA_DIR) + "/BASE_DATOS_PHISHING.csv";
static const std::string CSV_BENIGNAS = std::string(PROJECT_DATA_DIR) + "/BASE_DATOS_BENIGNAS.csv";
static const std::string CSV_UNIDA    = std::string(PROJECT_DATA_DIR) + "/BASE_DATOS_UNIDA.csv";
#else
static const std::string CSV_PHISHING = "";
static const std::string CSV_BENIGNAS = "";
static const std::string CSV_UNIDA    = "";
#endif

BaseDatosURL DataLoader::cargar() {
    // unimos las dos bases por defecto: phishing + benignas -> una sola base a analizar.
    BaseDatosURL db = unir(CSV_PHISHING, CSV_BENIGNAS);

    // si ambas bases existen, dejamos guardada la union en el tercer archivo.
    if (!CSV_UNIDA.empty() && db.total() > 0)
        guardarUnion(CSV_PHISHING, CSV_BENIGNAS, CSV_UNIDA);

    // si no se pudo abrir ningun CSV, caemos en datos demo para no quedar sin base.
    if (db.total() == 0)
        cargarDemo(db);

    return db;
}

BaseDatosURL DataLoader::unir(const std::string& rutaPhishing, const std::string& rutaBenignas) {
    BaseDatosURL db;

    // cargamos las dos bases sobre la MISMA base en memoria: el resultado equivale
    // al archivo original completo (la clasificacion de cada fila la marca CLASS_LABEL).
    if (!rutaPhishing.empty())
        cargarCSV(db, rutaPhishing);
    if (!rutaBenignas.empty())
        cargarCSV(db, rutaBenignas);

    return db;
}

bool DataLoader::guardarUnion(const std::string& rutaPhishing,
                              const std::string& rutaBenignas,
                              const std::string& rutaSalida) {
    QFile salida(QString::fromStdString(rutaSalida));
    if (!salida.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;   // no se pudo crear el tercer archivo

    QTextStream out(&salida);

    // copia las filas de un CSV al archivo de salida; conCabecera decide si se
    // escribe tambien la fila de encabezado (solo la queremos una vez, al inicio).
    auto copiar = [&out](const QString& ruta, bool conCabecera) {
        QFile in(ruta);
        if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream ts(&in);
        bool primera = true;
        while (!ts.atEnd()) {
            QString linea = ts.readLine();
            if (primera) {                 // la primera linea es el encabezado
                primera = false;
                if (!conCabecera) continue;
            }
            if (linea.trimmed().isEmpty()) continue;
            out << linea << "\n";
        }
        in.close();
    };

    copiar(QString::fromStdString(rutaPhishing), true);   // cabecera + filas phishing
    copiar(QString::fromStdString(rutaBenignas), false);  // solo filas benignas (sin repetir cabecera)
    salida.close();
    return true;
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

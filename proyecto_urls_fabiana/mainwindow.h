#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QBarSet>
#include <QValueAxis>
#include "data/DataStructures.h"
#include "data/DataLoader.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void mostrarResultado(const ResultadoAnalisis& r);

    // une las dos bases cargadas por el usuario y actualiza db + la etiqueta de estado
    void recargarBaseUnida();
    // refresca la etiqueta de estado con los totales de la base actual
    void actualizarEstadoBD();

    // crea una gráfica de barras con degradado verde->azul y la devuelve lista para insertar
    QWidget* crearGrafica(const QString& titulo, const QStringList& categorias,
                          QBarSet*& setSalida, QValueAxis*& ejeYSalida);

    QLineEdit* inputURL;
    QPushButton* btnAnalizar;
    QPushButton* btnCargarPhishing;   // base de datos 1: URLs de phishing
    QPushButton* btnCargarBenignas;   // base de datos 2: URLs benignas
    QLabel* lblClasificacion;
    QLabel* lblConfianza;
    QLabel* lblEstadoBD;              // muestra cuantos registros tiene la base unida

    // rutas de las dos bases que el usuario va cargando (se unen al tenerlas)
    QString rutaPhishing;
    QString rutaBenignas;

    // Gráfica 1: características numéricas de la URL analizada
    QBarSet*    setCaracteristicas;
    QValueAxis* ejeCaracteristicas;

    // Gráfica 2: distribución de votos de los K vecinos más cercanos
    QBarSet*    setDistribucion;
    QValueAxis* ejeDistribucion;

    BaseDatosURL  db;

// FIX: private slots estaba fuera de la clase (la llave } era demasiado temprana)
private slots:
    void onAnalizarClicked();
    void onCargarPhishingClicked();   // boton base 1
    void onCargarBenignasClicked();   // boton base 2 (nuevo)
};

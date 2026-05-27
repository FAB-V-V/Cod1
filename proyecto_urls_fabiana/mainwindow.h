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

    // crea una gráfica de barras con degradado verde->azul y la devuelve lista para insertar
    QWidget* crearGrafica(const QString& titulo, const QStringList& categorias,
                          QBarSet*& setSalida, QValueAxis*& ejeYSalida);

    QLineEdit* inputURL;
    QPushButton* btnAnalizar;
    QPushButton* btnCargarCSV;
    QLabel* lblClasificacion;
    QLabel* lblConfianza;

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
    void onCargarCSVClicked();
};

#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
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
    void actualizarEstadisticas();

    QLineEdit* inputURL;
    QPushButton* btnAnalizar;
    QPushButton* btnCargarCSV;
    QLabel* lblClasificacion;
    QLabel* lblConfianza;

    QProgressBar* url_length;
    QProgressBar* cant_dot;
    QProgressBar* cant_underscore;
    QProgressBar* cant_hyphen;
    QProgressBar* cant_query;

    QProgressBar* barBenigna;
    QProgressBar* barPhising;
    QProgressBar* barDefacement;
    QProgressBar* barMaleware;

    QTextEdit* areaStats

    BaseDatosURL  db;

// FIX: private slots estaba fuera de la clase (la llave } era demasiado temprana)
private slots:
    void onAnalizarClicked();
    void onCargarCSVClicked();
};

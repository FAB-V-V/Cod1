// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: declaración de MainWindow — input de URL, barras de parámetros y gráfica de tendencias

#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include "data/DataStructures.h"

class MainWindow : public QMainWindow {
    Q_OBJECT   

public:          // Constructor y destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:        //Toda la parte gráfica de Qt
    void setupUI();
    void mostrarResultado(const ResultadoAnalisis& r);

    QLineEdit*    inputURL;
    QPushButton*  btnAnalizar;
    QPushButton*  btnCargarCSV;
    QLabel*       lblClasificacion;
    QLabel*       lblConfianza;

    QProgressBar* url_length;
    QProgressBar* cant_dot;
    QProgressBar* cant_underscore;
    QProgressBar* cant_hyphen;
    QProgressBar* cant_query;

    QProgressBar* barBenigna;
    QProgressBar* barPhising;
    QProgressBar* barDefacement;
    QProgressBar* barMaleware;
    BaseDatosURL  db;
};


private slots:
    void onAnalizarClicked();
    void onCargarCSVClicked();
};

int comparacion_cant_length_URL_tendencia (int cant_length_URL,    int cant_length_tendencia);
int comparacion_cant_dot_URL_tendencia (int cant_dot_URL,       int cant_dot_tendencia);
int comparacion_cant_slash_URL_tendencia (int cant_slash_URL,     int cant_slash_tendencia);
int comparacion_cant_num_URL_tendencia (int cant_num_URL,       int cant_num_tendencia);
int comparacion_cant_hyphen_URL_tendencia  (int cant_hyphen_URL,    int cant_hyphen_tendencia);
int comparacion_cant_underscore_URL_tendencia  (int cant_underscore_URL,    int cant_underscore_tendencia);
int comparacion_cant_ampersand_URL_tendencia (int cant_ampersand_URL, int cant_ampersand_tendencia);
int comparacion_cant_https_URL_tendencia  (int cant_https_URL,     int cant_https_tendencia);
int comparacion_cant_at_URL_tendencia  (int cant_at_URL,        int cant_at_tendencia);
int comparacion_cant_percent_URL_tendencia (int cant_percent_URL,   int cant_percent_tendencia);

}

 #pragma once

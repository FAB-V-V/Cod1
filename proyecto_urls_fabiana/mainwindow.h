// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: declaración de MainWindow — input de URL, barras de parámetros y gráfica de tendencias

#include <QMainWindow>
#include <QPushButton>      
#include <QLineEdit>       
#include <QLabel>
#include <QComboBox>
#include <iostream>
#include <string>
using namespace std;

class MainWindow : public QMainWindow {
private:
    Q_object   

public:          // Constructor y destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:        //Toda la parte gráfica de Qt
    // OUTPUT: 
    QChart* Grafica_tendencias; // gráfica 1
    QLabel*      Informe_tendencia; //muestra un informe sobre como se comportan las 4 tendencias

    // INPUT: donde el usuario escribe la URL
    QLabel*      ingresar_URL;          // texto "Ingresa tu URL:"
    QLineEdit*   campo_URL;             // caja de texto donde escribe

    // OUTPUT: resultado del análisis
    QLabel*      Informe_URL_tendencia; // muestra la clasificación (phishing, malware, etc.)
    QChart* Grafica_URL_tendencia; // gráfica 2
};



tendencia analisis_tendencias (tendencia phising, tendencia maleware, tendencia defacement, tendencia benign);


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

    return 0;
}

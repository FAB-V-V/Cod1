// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: declaración de MainWindow — input de URL, barras de parámetros y gráfica de tendencias

#include <QMainWindow>
#include <QPushButton>      
#include <QLineEdit>       
#include <QLabel>
#include <QComboBox>

class MainWindow : public QMainWindow {
private:
    Q_OBJECT   

public:          // Constructor y destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:        //Toda la parte gráfica de Qt
    // OUTPUT: 
    QChart* Grafica_tendencias; // gráfica 1
    QLabel*      Informe_tendencia; //muestra un informe sobre como se comportan las 4 tendencias

    // INPUT: donde el usuario escribe la URL
    QLabel*      ingresar_URL;          // texto "Ingresa tu URL:"
    QLineEdit*   campo_URL;             // caja de texto

    // OUTPUT: resultado del análisis
    QLabel*      Informe_URL_tendencia; // muestra la clasificación (phishing, malware, defacement o benign)
    QChart* Grafica_URL_tendencia; // gráfica 2
};

class caracteristicas_URL {
private:
    string url;
    int cant_length;      
    int cant_dot;
    int cant_slash;
    int cant_num;
    int cant_hyphen;
    int cant_underscore;
    int cant_ampersand;
    int cant_https;
    int cant_at;
    int cant_percent;
 
public:
    // CONSTRUCTOR
    caracteristicas_URL(string _url) : url(_url) {
        cant_length = url.length();
        cant_dot = 0;
        cant_slash = 0;
        cant_num = 0;
        cant_hyphen = 0;
        cant_underscore = 0;
        cant_ampersand = 0;
        cant_https = 0;
        cant_at = 0;
        cant_percent = 0;
        contarCaracteres();
    }
 
    // DESTRUCTOR
    ~caracteristicas_URL() {}
 
    // MÉTODO: contar caracteres
    void contarCaracteres() {
        for (char c : url) {
            if (c == '.') {
                cant_dot++;
            }
            else if (c == '-') {
                cant_hyphen++;
            }
            else if (c == '/') {
                cant_slash++;
            }
            else if (isdigit(c)) { 
                cant_num++;
            }
            else if (c == '&') {
                cant_ampersand++;
            }
            else if (c == '_') {
                cant_underscore++;
            }
            else if (c == '@') {
                cant_at++;
            }
            else if (c == '%') {
                cant_percent++;
            }
        }
        
        // Detectar HTTPS
        if (url.find("https") != string::npos) {
            cant_https = 1;  // booleano: 1 = tiene HTTPS, 0 = no
        }
    }
 
    // GETTERS
    string get_url(){
        return url;
    };
    int get_cant_length() {
        return cant_length;
    };
    int get_cant_dot(){
        return cant_dot;
    };
    int get_cant_slash() {
        return cant_slash;
    };
    int get_cant_num() {
        return cant_num;
    };
    int get_cant_hyphen(){
        return cant_hyphen;
    };
    int get_cant_underscore() {
        return cant_underscore;
    };
    int get_cant_ampersand() {
        return cant_ampersand;
    }; 
    int get_cant_https() {
        return cant_https;
    };
    int get_cant_at() {
        return cant_at;
    };
    int get_cant_percent() {
        return cant_percent;
    };
 
    // SETTERS
    void set_url(string _url) {
        url = _url;
    };
    void set_cant_dot(int val) {
        cant_dot = val;
    };
    void set_cant_slash(int val) {
        cant_slash = val;
    };
    void set_cant_num(int val) {
        cant_num = val;
    };
    void set_cant_hyphen(int val) {
        cant_hyphen = val;
    };
    void set_cant_underscore(int val) {
        cant_underscore = val;
    };
    void set_cant_ampersand(int val) {
        cant_ampersand = val;
    };
    void set_cant_https(int val) {
        cant_https = val;
    };
    void set_cant_at(int val) {
        cant_at = val;
    };
    void set_cant_percent(int val) {
        cant_percent = val;
    };
};

        //     CLASE TENCENCIA
// Preguntar como se hace con listas 



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

}

 #endif
